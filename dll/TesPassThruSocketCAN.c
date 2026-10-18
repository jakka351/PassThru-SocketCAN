// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                             ___________              __                 __________                                      __                         
//                             \__    ___/___   _______/  |_  ___________  \______   \_______   ____   ______ ____   _____/  |_                       
//                               |    |_/ __ \ /  ___/\   __\/ __ \_  __ \  |     ___/\_  __ \_/ __ \ /  ___// __ \ /    \   __\                      
//                               |    |\  ___/ \___ \  |  | \  ___/|  | \/  |    |     |  | \/\  ___/ \___ \\  ___/|   |  \  |                        
//                               |____| \___  >____  > |__|  \___  >__|     |____|     |__|    \___  >____  >\___  >___|  /__|                        
//                                          \/     \/            \/                                \/     \/     \/     \/                            
//                               _________                    .__       .__  .__          __                                                          
//                              /   _____/_____   ____   ____ |__|____  |  | |__| _______/  |_                                                        
//                              \_____  \\____ \_/ __ \_/ ___\|  \__  \ |  | |  |/  ___/\   __\                                                       
//                              /        \  |_> >  ___/\  \___|  |/ __ \|  |_|  |\___ \  |  |                                                         
//                             /_______  /   __/ \___  >\___  >__(____  /____/__/____  > |__|                                                         
//                                     \/|__|        \/     \/        \/             \/                                                               
//                                _____          __                         __  .__                                                                   
//                               /  _  \  __ ___/  |_  ____   _____   _____/  |_|__|__  __ ____                                                       
//                              /  /_\  \|  |  \   __\/  _ \ /     \ /  _ \   __\  \  \/ // __ \                                                      
//                             /    |    \  |  /|  | (  <_> )  Y Y  (  <_> )  | |  |\   /\  ___/                                                      
//                             \____|__  /____/ |__|  \____/|__|_|  /\____/|__| |__| \_/  \___  >                                                     
//                                     \/                         \/                          \/                                                      
//                               _________      .__          __  .__                                                                                  
//                              /   _____/ ____ |  |  __ ___/  |_|__| ____   ____   ______                                                            
//                              \_____  \ /  _ \|  | |  |  \   __\  |/  _ \ /    \ /  ___/                                                            
//                              /        (  <_> )  |_|  |  /|  | |  (  <_> )   |  \\___ \                                                              
//                             /_______  /\____/|____/____/ |__| |__|\____/|___|  /____  >                                                           
//                                     \/                                       \/     \/                                                             
//
//
//
//                              Tester Present Specialist Automotive Solutions Open Source License (TPSAS-OSL)
//                              Version 1.0, 2025
//
//                              Copyright (c) 2025 Tester Present Specialist Automotive Solutions
//
//
//             Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
//             documentation files (the "Software"), to use, modify, and distribute the Software subject to the following conditions:
//
//          1. Attribution.
//            - The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//            - Credit must be given to "Tester Present Specialist Automotive Solutions" in any documentation, public notices, or product materials 
//              that acknowledge the use of the Software.//
//
//          2. Open Source Use Only.
//            - The Software, including any modifications or derivative works (collectively, “Derivatives”), must be distributed only as open source software.
//            - Derivatives or distributions of the Software must be licensed under this license or another license approved by the Open Source Initiative 
//              (OSI) that maintains equivalent open source freedoms.
//            - The Software and any Derivatives may not be used as a component in any closed source or proprietary software product.
//
//          3. Redistribution.
//            - Redistributions of the Software, in source or binary form, must reproduce the above attribution, this license, and the disclaimer below 
//              in the documentation and/or other materials provided with the distribution.
//            - Any modifications made to the Software must be clearly marked as such, and the modified files must carry prominent notices stating that
//              changes have been made.
//
//          4. No Warranty.
//            - THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//              FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT.
//            - IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES, OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
//              TORT, OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//          5. Termination.
//            - This license and the rights granted hereunder will terminate automatically upon any breach of the above conditions.
//            - Upon termination, you must cease all use and distribution of the Software and any Derivatives.
//
//          6. Entire Agreement.
//            - This license constitutes the entire agreement between you and Tester Present Specialist Automotive Solutions regarding the Software 
//              and supersedes any prior understandings or agreements, whether written or oral, regarding the subject matter herein.
//
//              By using, modifying, or distributing the Software, you agree to abide by all the terms and conditions of this license.
//
//
//                         
/*
 * TesPassThruSocketCAN.c  -  SAE J2534-1 (04.04) PassThru DLL over SocketCAN / socketcand
 * Tester Present Specialist Automotive Solutions  -  https://github.com/jakka351/PassThru-SocketCAN
 *
 * Presents a networked SocketCAN bus (a Raspberry Pi running socketcand) to any Windows J2534
 * application as a standard PassThru device. The DLL:
 *   - connects over TCP to socketcand (default port 29536) and attaches to an interface (can0)
 *     in raw mode, exactly as TES's own SocketCAN transport does;
 *   - implements CAN (raw) and ISO15765 (ISO-TP) channels, running the ISO 15765-2 segmentation
 *     and flow-control state machine IN THIS DLL (socketcand only carries raw frames);
 *   - reads its target (host/port/interface/bitrate/listen-only) from the registry, written by
 *     the TesSocketCANConfig setup app.
 *
 * SCOPE (v1):  CAN (protocol 5) and ISO15765 (protocol 6), 11- and 29-bit, physical addressing.
 *              K-line / J1850 are a different physical layer SocketCAN cannot carry -> returned
 *              as ERR_NOT_SUPPORTED so apps degrade gracefully.  CAN-FD and x64 are follow-ons.
 *
 * Build:  see build.bat  (cl /LD ... /link /DEF:TesPassThruSocketCAN.def ws2_32.lib advapi32.lib)
 */

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "advapi32.lib")

/* ===========================================================================
 *  J2534 constants
 * ========================================================================= */
/* Protocol IDs */
#define J1850VPW        1
#define J1850PWM        2
#define ISO9141         3
#define ISO14230        4
#define CAN             5
#define ISO15765        6

/* Error codes (J2534-1 §A) */
#define STATUS_NOERROR              0x00
#define ERR_NOT_SUPPORTED           0x01
#define ERR_INVALID_CHANNEL_ID      0x02
#define ERR_INVALID_PROTOCOL_ID     0x03
#define ERR_NULL_PARAMETER          0x04
#define ERR_INVALID_FLAGS           0x06
#define ERR_FAILED                  0x07
#define ERR_DEVICE_NOT_CONNECTED    0x08
#define ERR_TIMEOUT                 0x09
#define ERR_INVALID_MSG             0x0A
#define ERR_INVALID_DEVICE_ID       0x0F
#define ERR_DEVICE_IN_USE           0x10
#define ERR_INVALID_IOCTL_ID        0x11
#define ERR_BUFFER_EMPTY            0x12
#define ERR_BUFFER_FULL             0x13
#define ERR_EXCEEDED_LIMIT          0x15
#define ERR_INVALID_MSG_ID          0x17
#define ERR_NOT_OPEN                0x1A
#define ERR_CONCURRENT_API_CALL     0x1C

/* TxFlags / RxStatus */
#define TX_MSG_TYPE                 0x00000001
#define START_OF_MESSAGE            0x00000002   /* ISO15765 first-frame indication */
#define ISO15765_PADDING_ERROR      0x00000010
#define CAN_29BIT_ID                0x00000100
#define ISO15765_ADDR_TYPE          0x00000080   /* extended addressing (not supported v1) */
#define ISO15765_FRAME_PAD          0x00000040

/* Filter types */
#define PASS_FILTER                 0x00000001
#define BLOCK_FILTER                0x00000002
#define FLOW_CONTROL_FILTER         0x00000003

/* Ioctl IDs */
#define GET_CONFIG                  0x01
#define SET_CONFIG                  0x02
#define READ_VBATT                  0x03
#define CLEAR_TX_BUFFER             0x07
#define CLEAR_RX_BUFFER             0x08
#define CLEAR_MSG_FILTERS           0x0A

/* Config parameter IDs (subset) */
#define DATA_RATE                   0x01
#define LOOPBACK                    0x03
#define ISO15765_BS                 0x1E
#define ISO15765_STMIN              0x1F

#define MAX_MSG_DATA                4128
#define MAX_CHANNELS                4
#define MAX_FILTERS                 16
#define MAX_PERIODIC                8
#define RX_QUEUE_LEN                256
#define LINE_BUF                    1024

/* ===========================================================================
 *  J2534 PASSTHRU_MSG  (flat, 4128-byte data)  - must match the app's struct
 * ========================================================================= */
#pragma pack(push, 1)
typedef struct {
    uint32_t ProtocolID;
    uint32_t RxStatus;
    uint32_t TxFlags;
    uint32_t Timestamp;
    uint32_t DataSize;
    uint32_t ExtraDataIndex;
    uint8_t  Data[MAX_MSG_DATA];
} PASSTHRU_MSG;

typedef struct { uint32_t Parameter; uint32_t Value; } SCONFIG;
typedef struct { uint32_t NumOfParams; SCONFIG *ConfigPtr; } SCONFIG_LIST;
typedef struct { uint32_t NumOfBytes; uint8_t *BytePtr; } SBYTE_ARRAY;
#pragma pack(pop)

/* ===========================================================================
 *  Internal state
 * ========================================================================= */
typedef struct {
    uint32_t id;         /* 11- or 29-bit arbitration id */
    uint8_t  ext;        /* 1 = 29-bit */
    uint8_t  dlc;        /* 0..8 */
    uint8_t  data[8];
} canframe_t;

typedef struct {
    int      used;
    uint32_t type;       /* PASS/BLOCK/FLOW_CONTROL */
    uint32_t mask_id;    /* arbitration-id mask  */
    uint32_t patt_id;    /* arbitration-id pattern (the RX id we accept) */
    uint32_t fc_id;      /* flow-control TX id (the id we transmit requests/FC on) */
    uint8_t  fc_ext;     /* 29-bit flow-control id */
} filter_t;

/* One in-progress ISO-TP reassembly (keyed by source RX id) */
typedef struct {
    int      active;
    uint32_t rx_id;
    uint8_t  ext;
    uint16_t total;      /* expected total length from FF */
    uint16_t have;       /* bytes accumulated */
    uint8_t  next_sn;    /* expected sequence number */
    uint32_t last_ms;    /* GetTickCount at last FF/CF — for N_Cr stale-slot reclaim */
    uint8_t  buf[MAX_MSG_DATA];
} isotp_rx_t;

typedef struct {
    int           used;
    uint32_t      protocol;      /* CAN or ISO15765 */
    uint32_t      flags;         /* connect flags (CAN_29BIT_ID etc.) */
    uint32_t      baud;
    int           loopback;
    int           pad;           /* ISO15765 frame padding on */
    filter_t      filters[MAX_FILTERS];
    isotp_rx_t    reasm[MAX_FILTERS];   /* touched ONLY by the RX thread -> no lock needed */
    /* rx message queue (completed PASSTHRU_MSGs ready for the app) */
    PASSTHRU_MSG  rxq[RX_QUEUE_LEN];
    int           rq_head, rq_tail, rq_count;
    CRITICAL_SECTION rq_lock;           /* guards rxq + rq_* */
    CRITICAL_SECTION filt_lock;         /* guards filters[] (RX thread vs app Start/Stop/Clear) — leaf lock */
    CRITICAL_SECTION iso_tx_lock;       /* serialises one ISO-TP transmit sequence per channel */
    /* per-channel ISO-TP flow-control capture (set by RX thread, read by this channel's TX) */
    HANDLE            fc_event;
    volatile uint32_t fc_from_id;       /* source id the captured FC arrived on */
    volatile uint8_t  fc_fs;            /* 0=CTS 1=WAIT 2=OVFLW */
    volatile uint8_t  fc_bs;
    volatile uint8_t  fc_stmin;
} channel_t;

static channel_t  g_chan[MAX_CHANNELS];
static int        g_device_open = 0;
static SOCKET     g_sock = INVALID_SOCKET;
static HANDLE     g_rx_thread = NULL;
static volatile LONG g_rx_run = 0;
static CRITICAL_SECTION g_tx_lock;      /* serialises raw writes to the one socketcand socket */
static int        g_cs_init = 0;
static char       g_last_error[80] = "No error";

/* config (from registry) */
static char       g_host[128] = "10.42.0.1";
static int        g_port = 29536;
static char       g_iface[32] = "can0";
static uint32_t   g_bitrate = 500000;
static int        g_listen_only = 0;

static void set_err(const char *s) { strncpy(g_last_error, s, sizeof(g_last_error)-1); g_last_error[sizeof(g_last_error)-1] = 0; }

/* ===========================================================================
 *  Registry config
 * ========================================================================= */
static void load_config(void)
{
    HKEY k;
    /* 32-bit DLL -> HKLM\SOFTWARE is auto-redirected to WOW6432Node */
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
            "SOFTWARE\\Tester Present\\PassThru-SocketCAN", 0, KEY_READ, &k) == ERROR_SUCCESS) {
        DWORD sz, type, dw;
        /* RegQueryValueExA does not guarantee a NUL terminator; force one and ignore
         * wrong-typed values rather than trusting a mistyped/short REG_SZ. */
        type = 0; sz = sizeof(g_host);
        if (RegQueryValueExA(k, "Host", NULL, &type, (LPBYTE)g_host, &sz) == ERROR_SUCCESS && type == REG_SZ)
            g_host[(sz < sizeof(g_host)) ? sz : sizeof(g_host)-1] = 0;
        g_host[sizeof(g_host)-1] = 0;
        type = 0; sz = sizeof(g_iface);
        if (RegQueryValueExA(k, "Interface", NULL, &type, (LPBYTE)g_iface, &sz) == ERROR_SUCCESS && type == REG_SZ)
            g_iface[(sz < sizeof(g_iface)) ? sz : sizeof(g_iface)-1] = 0;
        g_iface[sizeof(g_iface)-1] = 0;
        sz = sizeof(dw); if (RegQueryValueExA(k, "Port",      NULL, &type, (LPBYTE)&dw, &sz) == ERROR_SUCCESS && type == REG_DWORD && dw) g_port = (int)dw;
        sz = sizeof(dw); if (RegQueryValueExA(k, "Bitrate",   NULL, &type, (LPBYTE)&dw, &sz) == ERROR_SUCCESS && type == REG_DWORD && dw) g_bitrate = dw;
        sz = sizeof(dw); if (RegQueryValueExA(k, "ListenOnly",NULL, &type, (LPBYTE)&dw, &sz) == ERROR_SUCCESS && type == REG_DWORD)       g_listen_only = (int)dw;
        RegCloseKey(k);
    }
}

/* ===========================================================================
 *  socketcand TCP transport
 * ========================================================================= */
static int sc_write(const char *s)
{
    int len = (int)strlen(s), sent = 0;
    EnterCriticalSection(&g_tx_lock);
    while (sent < len) {
        int n = send(g_sock, s + sent, len - sent, 0);
        if (n <= 0) { LeaveCriticalSection(&g_tx_lock); return 0; }
        sent += n;
    }
    LeaveCriticalSection(&g_tx_lock);
    return 1;
}

/* Blocking read of one "< ... >" message into out (NUL-terminated). timeout in ms. 1=ok 0=fail */
static int sc_read_msg(char *out, int cap, int timeout_ms)
{
    int len = 0; char c;
    DWORD tv = (DWORD)timeout_ms;
    setsockopt(g_sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
    /* find '<' */
    for (;;) {
        int n = recv(g_sock, &c, 1, 0);
        if (n <= 0) return 0;
        if (c == '<') break;
    }
    while (len < cap - 1) {
        int n = recv(g_sock, &c, 1, 0);
        if (n <= 0) return 0;
        if (c == '>') { out[len] = 0; return 1; }
        out[len++] = c;
    }
    out[cap-1] = 0;
    return 1;
}

/* socketcand's success reply to open/rawmode is exactly "< ok >". A substring test on
 * "ok" also matches error replies that happen to contain those letters, so compare the
 * trimmed payload as a whole token. */
static int reply_is_ok(const char *s)
{
    while (*s == ' ') s++;
    if (s[0] != 'o' || s[1] != 'k') return 0;
    s += 2;
    while (*s == ' ') s++;
    return *s == 0;
}

static int sc_command(const char *cmd, int timeout_ms)
{
    char reply[LINE_BUF];
    if (!sc_write(cmd)) return 0;
    if (!sc_read_msg(reply, sizeof(reply), timeout_ms)) return 0;
    return reply_is_ok(reply);
}

static int sc_connect(void)
{
    struct addrinfo hints, *res = NULL, *p;
    char portstr[16];
    char hi[LINE_BUF];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    sprintf(portstr, "%d", g_port);
    if (getaddrinfo(g_host, portstr, &hints, &res) != 0) { set_err("socketcand host not resolvable"); return 0; }

    for (p = res; p; p = p->ai_next) {
        g_sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (g_sock == INVALID_SOCKET) continue;
        /* 5s connect timeout via non-blocking + select */
        u_long nb = 1; ioctlsocket(g_sock, FIONBIO, &nb);
        connect(g_sock, p->ai_addr, (int)p->ai_addrlen);
        {
            fd_set wfd; struct timeval tv; FD_ZERO(&wfd); FD_SET(g_sock, &wfd);
            tv.tv_sec = 5; tv.tv_usec = 0;
            if (select(0, NULL, &wfd, NULL, &tv) == 1) { nb = 0; ioctlsocket(g_sock, FIONBIO, &nb); break; }
        }
        closesocket(g_sock); g_sock = INVALID_SOCKET;
    }
    freeaddrinfo(res);
    if (g_sock == INVALID_SOCKET) { set_err("could not reach socketcand (timeout)"); return 0; }

    { int one = 1; setsockopt(g_sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&one, sizeof(one)); }

    /* Handshake: "< hi >" (tolerate), open bus, rawmode */
    sc_read_msg(hi, sizeof(hi), 1500);
    {
        char cmd[64];
        sprintf(cmd, "< open %s >", g_iface);
        if (!sc_command(cmd, 2000)) { set_err("socketcand rejected 'open'"); return 0; }
        if (!sc_command("< rawmode >", 2000)) { set_err("socketcand rejected 'rawmode'"); return 0; }
    }
    return 1;
}

/* Build and send "< send ID DLC B0 B1 ... >" */
static int sc_send_frame(const canframe_t *f)
{
    char line[128]; int p = 0; int i;
    uint32_t id = f->id & (f->ext ? 0x1FFFFFFFu : 0x7FFu);
    if (g_listen_only) return 0;
    p += sprintf(line + p, "< send %0*X %u", f->ext ? 8 : 3, id, (unsigned)f->dlc);
    for (i = 0; i < f->dlc && i < 8; i++) p += sprintf(line + p, " %02X", f->data[i]);
    p += sprintf(line + p, " >");
    return sc_write(line);
}

/* Parse the contents between "< >" of a "frame ID TS DATA" message. 1=ok.
 * socketcand emits a contiguous hex blob for DATA; spaced bytes are tolerated.
 * DATA may be absent (zero-length frame). */
static int sc_parse_frame(const char *msg, canframe_t *f)
{
    const char *p = msg;
    char idbuf[16];
    int i;

    while (*p == ' ') p++;
    if (strncmp(p, "frame", 5) != 0) return 0;
    p += 5;

    while (*p == ' ') p++;                                   /* token 1: id */
    for (i = 0; i < 15 && *p && *p != ' '; i++) idbuf[i] = *p++;
    idbuf[i] = 0;
    if (i == 0) return 0;

    while (*p == ' ') p++;                                   /* token 2: timestamp (skip) */
    while (*p && *p != ' ') p++;

    memset(f, 0, sizeof(*f));
    f->id  = (uint32_t)strtoul(idbuf, NULL, 16);
    f->ext = (strlen(idbuf) > 3 || f->id > 0x7FF) ? 1 : 0;
    f->id &= f->ext ? 0x1FFFFFFFu : 0x7FFu;

    {                                                        /* remainder: hex data */
        int n = 0;
        while (*p && n < 8) {
            while (*p == ' ') p++;
            if (!isxdigit((unsigned char)p[0]) || !isxdigit((unsigned char)p[1])) break;
            { char bb[3]; bb[0] = p[0]; bb[1] = p[1]; bb[2] = 0; f->data[n++] = (uint8_t)strtoul(bb, NULL, 16); }
            p += 2;
        }
        f->dlc = (uint8_t)n;
    }
    return 1;
}

/* ===========================================================================
 *  RX queue helpers (per channel)
 * ========================================================================= */
static void rq_push(channel_t *ch, const PASSTHRU_MSG *m)
{
    EnterCriticalSection(&ch->rq_lock);
    if (ch->rq_count < RX_QUEUE_LEN) {
        ch->rxq[ch->rq_tail] = *m;
        ch->rq_tail = (ch->rq_tail + 1) % RX_QUEUE_LEN;
        ch->rq_count++;
    } else {
        /* drop oldest */
        ch->rq_head = (ch->rq_head + 1) % RX_QUEUE_LEN;
        ch->rxq[ch->rq_tail] = *m;
        ch->rq_tail = (ch->rq_tail + 1) % RX_QUEUE_LEN;
    }
    LeaveCriticalSection(&ch->rq_lock);
}

static int rq_pop(channel_t *ch, PASSTHRU_MSG *out)
{
    int ok = 0;
    EnterCriticalSection(&ch->rq_lock);
    if (ch->rq_count > 0) {
        *out = ch->rxq[ch->rq_head];
        ch->rq_head = (ch->rq_head + 1) % RX_QUEUE_LEN;
        ch->rq_count--;
        ok = 1;
    }
    LeaveCriticalSection(&ch->rq_lock);
    return ok;
}

/* Build a PASSTHRU_MSG from a CAN id + payload (ISO15765/CAN layout: [4-byte id BE][data]) */
static void make_msg(PASSTHRU_MSG *m, uint32_t protocol, uint32_t rxstatus,
                     uint32_t id, int ext, const uint8_t *payload, uint32_t plen)
{
    memset(m, 0, sizeof(*m));
    m->ProtocolID = protocol;
    m->RxStatus   = rxstatus | (ext ? CAN_29BIT_ID : 0);
    m->Timestamp  = (uint32_t)(GetTickCount64() * 1000ULL);  /* 32-bit us field; GetTickCount()*1000 overflowed at ~71min uptime */
    m->Data[0] = (uint8_t)(id >> 24); m->Data[1] = (uint8_t)(id >> 16);
    m->Data[2] = (uint8_t)(id >> 8);  m->Data[3] = (uint8_t)id;
    if (plen > MAX_MSG_DATA - 4) plen = MAX_MSG_DATA - 4;
    if (plen && payload) memcpy(m->Data + 4, payload, plen);
    m->DataSize = 4 + plen;
}

/* ===========================================================================
 *  Filtering + ISO-TP reception
 * ========================================================================= */
/* Returns 1 and copies the matched (non-block) filter into *out, else 0. The copy is taken
 * under filt_lock so the RX thread never races the app's Start/Stop/Clear filter calls, and
 * the caller holds no lock afterwards (filt_lock is a leaf — never nested with tx/rq locks). */
static int match_filter(channel_t *ch, const canframe_t *f, filter_t *out)
{
    int i, matched = 0;
    EnterCriticalSection(&ch->filt_lock);
    for (i = 0; i < MAX_FILTERS; i++) {
        filter_t *fl = &ch->filters[i];
        if (!fl->used) continue;
        if ((f->id & fl->mask_id) == (fl->patt_id & fl->mask_id)) {
            if (fl->type == BLOCK_FILTER) { matched = 0; break; }  /* explicitly blocked */
            *out = *fl; matched = 1; break;
        }
    }
    LeaveCriticalSection(&ch->filt_lock);
    return matched;
}

/* 1 if the channel has any filter installed (RX gate for raw CAN). */
static int any_filter(channel_t *ch)
{
    int k, any = 0;
    EnterCriticalSection(&ch->filt_lock);
    for (k = 0; k < MAX_FILTERS; k++) if (ch->filters[k].used) { any = 1; break; }
    LeaveCriticalSection(&ch->filt_lock);
    return any;
}

/* Send an ISO-TP flow-control frame (CTS, BS=0, STmin=0) on fc_id */
static void send_fc(const filter_t *fl)
{
    canframe_t fc;
    memset(&fc, 0, sizeof(fc));
    fc.id = fl->fc_id; fc.ext = fl->fc_ext; fc.dlc = 8;
    fc.data[0] = 0x30; fc.data[1] = 0x00; fc.data[2] = 0x00;  /* CTS, BS=0, STmin=0 */
    sc_send_frame(&fc);
}

/* Feed one received CAN frame into a channel (post-filter) */
static void channel_rx(channel_t *ch, const canframe_t *f)
{
    filter_t fl;
    PASSTHRU_MSG m;

    if (ch->protocol == CAN) {
        /* Raw CAN: once ANY filter is installed, a frame must pass one. (Keying only on
         * filters[0].used leaked everything when the app's filter landed in a later slot.) */
        if (!match_filter(ch, f, &fl) && any_filter(ch)) return;
        make_msg(&m, CAN, 0, f->id, f->ext, f->data, f->dlc);
        rq_push(ch, &m);
        return;
    }

    /* ISO15765 */
    if (!match_filter(ch, f, &fl)) return;       /* not one of our flow-control conversations */
    if (f->dlc < 1) return;

    {
        uint8_t pci = f->data[0] >> 4;
        if (pci == 0x0) {                        /* Single Frame */
            uint8_t len = f->data[0] & 0x0F;
            if (len == 0 || len > 7 || (uint8_t)(len + 1) > f->dlc) return;  /* malformed SF */
            make_msg(&m, ISO15765, 0, f->id, f->ext, f->data + 1, len);
            rq_push(ch, &m);
        }
        else if (pci == 0x1) {                   /* First Frame */
            int i;
            isotp_rx_t *r = NULL;
            uint32_t now = GetTickCount();
            uint16_t total = (uint16_t)(((f->data[0] & 0x0F) << 8) | f->data[1]);
            if (total < 8) return;                           /* <8 => SF territory / escape form (unsupported v1) */
            if (total > MAX_MSG_DATA) total = MAX_MSG_DATA;  /* clamp BEFORE storing so CF writes can't overrun buf */
            /* reuse a slot already reassembling this id (duplicate / restarted FF) ... */
            for (i = 0; i < MAX_FILTERS; i++) if (ch->reasm[i].active && ch->reasm[i].rx_id == f->id) { r = &ch->reasm[i]; break; }
            /* ... else a free slot ... */
            if (!r) for (i = 0; i < MAX_FILTERS; i++) if (!ch->reasm[i].active) { r = &ch->reasm[i]; break; }
            /* ... else reclaim the stalest slot (N_Cr elapsed) so orphaned FFs can't wedge reassembly */
            if (!r) for (i = 0; i < MAX_FILTERS; i++) if ((uint32_t)(now - ch->reasm[i].last_ms) > 1000u) { r = &ch->reasm[i]; break; }
            if (!r) return;
            r->active = 1; r->rx_id = f->id; r->ext = f->ext; r->total = total; r->have = 0; r->next_sn = 1; r->last_ms = now;
            { int c = f->dlc - 2; if (c > (int)total) c = (int)total; if (c > 0) { memcpy(r->buf, f->data + 2, c); r->have = (uint16_t)c; } }
            /* J2534 first-frame indication: id only, START_OF_MESSAGE */
            make_msg(&m, ISO15765, START_OF_MESSAGE, f->id, f->ext, NULL, 0);
            rq_push(ch, &m);
            send_fc(&fl);                         /* we are the receiver -> grant flow control */
        }
        else if (pci == 0x2) {                   /* Consecutive Frame */
            int i; isotp_rx_t *r = NULL;
            for (i = 0; i < MAX_FILTERS; i++) { if (ch->reasm[i].active && ch->reasm[i].rx_id == f->id) { r = &ch->reasm[i]; break; } }
            if (!r) return;
            if ((f->data[0] & 0x0F) != r->next_sn) { r->active = 0; return; }  /* sequence gap -> abandon */
            r->next_sn = (uint8_t)((r->next_sn + 1) & 0x0F);
            r->last_ms = GetTickCount();
            {
                int c = f->dlc - 1;
                int room = (int)r->total - (int)r->have;
                if (c > room) c = room;
                if (c > 0) { memcpy(r->buf + r->have, f->data + 1, c); r->have = (uint16_t)(r->have + c); }
            }
            if (r->have >= r->total) {
                make_msg(&m, ISO15765, 0, r->rx_id, r->ext, r->buf, r->total);
                rq_push(ch, &m);
                r->active = 0;
            }
        }
        else if (pci == 0x3) {                   /* Flow Control from the ECU (for this channel's multi-frame TX) */
            ch->fc_from_id = f->id;
            ch->fc_fs      = (uint8_t)(f->data[0] & 0x0F);
            ch->fc_bs      = f->dlc > 1 ? f->data[1] : 0;
            ch->fc_stmin   = f->dlc > 2 ? f->data[2] : 0;
            if (ch->fc_event) SetEvent(ch->fc_event);
        }
    }
}

/* ===========================================================================
 *  RX thread: pull frames off socketcand, fan out to open channels
 * ========================================================================= */
static DWORD WINAPI rx_thread(LPVOID arg)
{
    char msg[LINE_BUF];
    (void)arg;
    while (InterlockedCompareExchange(&g_rx_run, 1, 1)) {
        canframe_t f;
        if (!sc_read_msg(msg, sizeof(msg), 500)) {
            if (WSAGetLastError() == WSAETIMEDOUT) continue;   /* idle bus */
            break;                                             /* socket dead */
        }
        if (!sc_parse_frame(msg, &f)) continue;
        {
            int c;
            for (c = 0; c < MAX_CHANNELS; c++)
                if (g_chan[c].used) channel_rx(&g_chan[c], &f);
        }
    }
    return 0;
}

/* Transmit an ISO15765 message: Single Frame, or First Frame + flow-controlled Consecutive Frames. */
static uint32_t iso15765_send(channel_t *ch, uint32_t id, int ext, const uint8_t *payload, uint32_t len)
{
    canframe_t f;
    uint32_t off = 0;
    uint8_t  sn = 1;
    int      padlen = ch->pad ? 1 : 0;
    int      wft = 0;                                  /* consecutive no-progress FC counter (N_WFTmax) */
    uint32_t exp_src = 0; int have_exp = 0; int i;
    uint32_t rc;

    /* FF_DL is 12 bits here (no escape form in v1). Reject oversize up front so the FF
     * length never truncates and the CF loop can never read past the caller's Data[]. */
    if (len > 4095) { set_err("ISO-TP message exceeds 4095 bytes (v1)"); return ERR_EXCEEDED_LIMIT; }

    /* The ECU's flow control comes back on the response id. Derive it from the flow-control
     * filter the app installed for this TX id (fc_id -> patt_id), so a stray FC from another
     * conversation can't satisfy our wait. */
    EnterCriticalSection(&ch->filt_lock);
    for (i = 0; i < MAX_FILTERS; i++)
        if (ch->filters[i].used && ch->filters[i].type == FLOW_CONTROL_FILTER && ch->filters[i].fc_id == id)
            { exp_src = ch->filters[i].patt_id; have_exp = 1; break; }
    LeaveCriticalSection(&ch->filt_lock);

    /* Serialise the whole transmit sequence per channel so a second writer (or a raw SF)
     * can't inject frames between our FF and CFs on the same bus. */
    EnterCriticalSection(&ch->iso_tx_lock);

    if (len <= 7) {                                    /* Single Frame */
        memset(&f, 0, sizeof(f)); f.id = id; f.ext = ext;
        f.data[0] = (uint8_t)len;                      /* SF, length in low nibble */
        memcpy(f.data + 1, payload, len);
        f.dlc = padlen ? 8 : (uint8_t)(1 + len);
        rc = sc_send_frame(&f) ? STATUS_NOERROR : ERR_FAILED;
        goto done;
    }

    if (ch->fc_event) ResetEvent(ch->fc_event);        /* arm before the FF so a stale FC can't fire */

    memset(&f, 0, sizeof(f)); f.id = id; f.ext = ext;  /* First Frame */
    f.data[0] = (uint8_t)(0x10 | ((len >> 8) & 0x0F));
    f.data[1] = (uint8_t)(len & 0xFF);
    memcpy(f.data + 2, payload, 6);
    f.dlc = 8;
    if (!sc_send_frame(&f)) { rc = ERR_FAILED; goto done; }
    off = 6;

    for (;;) {                                         /* one iteration per flow-control block */
        uint8_t bs, stmin; uint32_t in_block = 0;
        if (!ch->fc_event || WaitForSingleObject(ch->fc_event, 1000) != WAIT_OBJECT_0) { rc = ERR_TIMEOUT; goto done; }
        ResetEvent(ch->fc_event);
        if (have_exp && ch->fc_from_id != exp_src) { if (++wft > 4) { rc = ERR_TIMEOUT; goto done; } continue; }  /* not our ECU */
        if (ch->fc_fs == 1) { if (++wft > 4) { rc = ERR_TIMEOUT; goto done; } continue; }  /* WAIT -> bounded by N_WFTmax */
        if (ch->fc_fs == 2) { rc = ERR_FAILED; goto done; }           /* OVFLW -> ECU can't receive */
        wft = 0;
        bs = ch->fc_bs; stmin = ch->fc_stmin;          /* CTS */

        while (off < len) {
            int c = (int)(len - off); if (c > 7) c = 7;
            memset(&f, 0, sizeof(f)); f.id = id; f.ext = ext;
            f.data[0] = (uint8_t)(0x20 | (sn & 0x0F));
            memcpy(f.data + 1, payload + off, c);
            f.dlc = padlen ? 8 : (uint8_t)(1 + c);
            if (!sc_send_frame(&f)) { rc = ERR_FAILED; goto done; }
            off += c; sn = (uint8_t)((sn + 1) & 0x0F); in_block++;
            /* ISO 15765-2 STmin: 0x00..0x7F = ms; 0xF1..0xF9 = 100..900 us; else reserved -> max */
            if      (stmin == 0)                      { /* no separation time */ }
            else if (stmin <= 0x7F)                   Sleep(stmin);
            else if (stmin >= 0xF1 && stmin <= 0xF9)  Sleep(1);
            else                                      Sleep(127);
            if (bs != 0 && in_block >= bs && off < len) break;   /* block full -> await next FC */
        }
        if (off >= len) break;
    }
    rc = STATUS_NOERROR;

done:
    LeaveCriticalSection(&ch->iso_tx_lock);
    return rc;
}

/* ===========================================================================
 *  Lifecycle helpers
 * ========================================================================= */
static void ensure_cs(void)
{
    if (!g_cs_init) {
        int i;
        InitializeCriticalSection(&g_tx_lock);
        for (i = 0; i < MAX_CHANNELS; i++) {
            InitializeCriticalSection(&g_chan[i].rq_lock);
            InitializeCriticalSection(&g_chan[i].filt_lock);
            InitializeCriticalSection(&g_chan[i].iso_tx_lock);
            g_chan[i].fc_event = CreateEventA(NULL, TRUE /*manual-reset*/, FALSE, NULL);
        }
        g_cs_init = 1;
    }
}

/* Zero a channel's data members WITHOUT touching rq_lock. rq_lock is initialised once in
 * ensure_cs() and must survive open/close — memset-ing over a live CRITICAL_SECTION
 * corrupts it, and the next EnterCriticalSection then deadlocks the RX path on first use. */
static void reset_channel(channel_t *ch)
{
    /* Called only with the RX thread stopped (open before thread start, close after join),
     * so no live reader races these writes. */
    EnterCriticalSection(&ch->rq_lock);
    ch->used = 0; ch->protocol = 0; ch->flags = 0; ch->baud = 0;
    ch->loopback = 0; ch->pad = 0;
    memset(ch->filters, 0, sizeof(ch->filters));
    memset(ch->reasm,   0, sizeof(ch->reasm));
    ch->rq_head = ch->rq_tail = ch->rq_count = 0;
    ch->fc_from_id = 0; ch->fc_fs = 0; ch->fc_bs = 0; ch->fc_stmin = 0;
    if (ch->fc_event) ResetEvent(ch->fc_event);
    LeaveCriticalSection(&ch->rq_lock);
}

static void teardown_transport(void)
{
    InterlockedExchange(&g_rx_run, 0);
    if (g_rx_thread) { WaitForSingleObject(g_rx_thread, 1500); CloseHandle(g_rx_thread); g_rx_thread = NULL; }
    if (g_sock != INVALID_SOCKET) { closesocket(g_sock); g_sock = INVALID_SOCKET; }
}

/* ===========================================================================
 *  DllMain / Winsock
 * ========================================================================= */
static int g_wsa = 0;
BOOL WINAPI DllMain(HINSTANCE h, DWORD reason, LPVOID r)
{
    (void)r;
    if (reason == DLL_PROCESS_ATTACH) {
        /* Loader-lock-safe work only. WSAStartup loads provider DLLs and must NOT run
         * here; it is started lazily in PassThruOpen. CreateEvent + InitializeCriticalSection
         * are kernel32 and documented-safe in DllMain. */
        DisableThreadLibraryCalls(h);
        ensure_cs();   /* creates per-channel locks + flow-control events */
    } else if (reason == DLL_PROCESS_DETACH) {
        /* Never join the RX thread or call WSACleanup under the loader lock — both can
         * deadlock. Only signal the thread to stop; PassThruClose does the real teardown,
         * and the OS reclaims the socket/thread/Winsock at process exit. */
        InterlockedExchange(&g_rx_run, 0);
    }
    return TRUE;
}

/* ===========================================================================
 *  J2534 exports
 * ========================================================================= */
__declspec(dllexport) long __stdcall PassThruOpen(void *pName, unsigned long *pDeviceID)
{
    int i;
    (void)pName;
    if (!pDeviceID) return ERR_NULL_PARAMETER;
    ensure_cs();
    if (g_device_open) return ERR_DEVICE_IN_USE;

    load_config();
    if (!g_wsa) {                                   /* lazy Winsock init (moved out of DllMain) */
        WSADATA wd;
        if (WSAStartup(MAKEWORD(2,2), &wd) != 0) { set_err("WSAStartup failed"); return ERR_FAILED; }
        g_wsa = 1;
    }
    if (!sc_connect()) return ERR_DEVICE_NOT_CONNECTED;

    for (i = 0; i < MAX_CHANNELS; i++) reset_channel(&g_chan[i]);

    InterlockedExchange(&g_rx_run, 1);
    g_rx_thread = CreateThread(NULL, 0, rx_thread, NULL, 0, NULL);
    if (!g_rx_thread) { teardown_transport(); set_err("rx thread failed"); return ERR_FAILED; }

    g_device_open = 1;
    *pDeviceID = 1;
    set_err("No error");
    return STATUS_NOERROR;
}

__declspec(dllexport) long __stdcall PassThruClose(unsigned long DeviceID)
{
    int i;
    (void)DeviceID;
    if (!g_device_open) return ERR_INVALID_DEVICE_ID;
    teardown_transport();
    for (i = 0; i < MAX_CHANNELS; i++) reset_channel(&g_chan[i]);
    g_device_open = 0;
    return STATUS_NOERROR;
}

__declspec(dllexport) long __stdcall PassThruConnect(
    unsigned long DeviceID, unsigned long ProtocolID,
    unsigned long Flags, unsigned long BaudRate, unsigned long *pChannelID)
{
    int i;
    (void)DeviceID;
    if (!pChannelID) return ERR_NULL_PARAMETER;
    if (!g_device_open) return ERR_INVALID_DEVICE_ID;
    if (ProtocolID != CAN && ProtocolID != ISO15765) { set_err("only CAN and ISO15765 are supported over SocketCAN"); return ERR_NOT_SUPPORTED; }

    for (i = 0; i < MAX_CHANNELS; i++) if (!g_chan[i].used) break;
    if (i == MAX_CHANNELS) return ERR_EXCEEDED_LIMIT;

    {
        channel_t *ch = &g_chan[i];
        memset(ch->filters, 0, sizeof(ch->filters));
        memset(ch->reasm, 0, sizeof(ch->reasm));
        ch->rq_head = ch->rq_tail = ch->rq_count = 0;
        ch->protocol = ProtocolID;
        ch->flags = Flags;
        ch->baud = BaudRate ? BaudRate : g_bitrate;
        ch->loopback = 0;
        ch->pad = (Flags & ISO15765_FRAME_PAD) ? 1 : 0;
        /* publish used last so the RX thread never sees a half-initialised channel */
        MemoryBarrier();
        ch->used = 1;
    }
    *pChannelID = (unsigned long)(i + 1);     /* 1-based channel id */
    return STATUS_NOERROR;
}

static channel_t *chan_from_id(unsigned long id)
{
    if (id < 1 || id > MAX_CHANNELS) return NULL;
    if (!g_chan[id-1].used) return NULL;
    return &g_chan[id-1];
}

__declspec(dllexport) long __stdcall PassThruDisconnect(unsigned long ChannelID)
{
    channel_t *ch = chan_from_id(ChannelID);
    if (!ch) return ERR_INVALID_CHANNEL_ID;
    EnterCriticalSection(&ch->rq_lock);
    ch->used = 0; ch->rq_count = ch->rq_head = ch->rq_tail = 0;
    LeaveCriticalSection(&ch->rq_lock);
    return STATUS_NOERROR;
}

__declspec(dllexport) long __stdcall PassThruReadMsgs(
    unsigned long ChannelID, void *pMsg, unsigned long *pNumMsgs, unsigned long Timeout)
{
    channel_t *ch = chan_from_id(ChannelID);
    PASSTHRU_MSG *out = (PASSTHRU_MSG *)pMsg;
    unsigned long want, got = 0;
    DWORD deadline;
    if (!pMsg || !pNumMsgs) return ERR_NULL_PARAMETER;
    if (!ch) return ERR_INVALID_CHANNEL_ID;
    want = *pNumMsgs;
    deadline = GetTickCount() + Timeout;

    for (;;) {
        while (got < want && rq_pop(ch, &out[got])) got++;
        if (got >= want) break;
        if (Timeout == 0) break;
        if ((long)(GetTickCount() - deadline) >= 0) break;
        Sleep(1);
    }
    *pNumMsgs = got;
    if (got == 0) return ERR_BUFFER_EMPTY;
    return STATUS_NOERROR;
}

__declspec(dllexport) long __stdcall PassThruWriteMsgs(
    unsigned long ChannelID, void *pMsg, unsigned long *pNumMsgs, unsigned long Timeout)
{
    channel_t *ch = chan_from_id(ChannelID);
    PASSTHRU_MSG *in = (PASSTHRU_MSG *)pMsg;
    unsigned long n, i, sent = 0;
    (void)Timeout;
    if (!pMsg || !pNumMsgs) return ERR_NULL_PARAMETER;
    if (!ch) return ERR_INVALID_CHANNEL_ID;
    n = *pNumMsgs;

    for (i = 0; i < n; i++) {
        PASSTHRU_MSG *m = &in[i];
        uint32_t id; int ext; uint32_t plen;
        if (m->DataSize < 4 || m->DataSize > MAX_MSG_DATA) { continue; }  /* malformed / would read past Data[] */
        id = ((uint32_t)m->Data[0] << 24) | ((uint32_t)m->Data[1] << 16) |
             ((uint32_t)m->Data[2] << 8)  |  (uint32_t)m->Data[3];
        ext = (m->TxFlags & CAN_29BIT_ID) ? 1 : (id > 0x7FF);
        plen = m->DataSize - 4;

        if (ch->protocol == CAN) {
            canframe_t f; memset(&f, 0, sizeof(f));
            f.id = id; f.ext = ext;
            f.dlc = (uint8_t)(plen > 8 ? 8 : plen);
            memcpy(f.data, m->Data + 4, f.dlc);
            if (sc_send_frame(&f)) sent++;
        } else { /* ISO15765 */
            if (iso15765_send(ch, id, ext, m->Data + 4, plen) == STATUS_NOERROR) sent++;
        }
    }
    *pNumMsgs = sent;
    return (sent == n) ? STATUS_NOERROR : (sent ? STATUS_NOERROR : ERR_FAILED);
}

__declspec(dllexport) long __stdcall PassThruStartMsgFilter(
    unsigned long ChannelID, unsigned long FilterType,
    void *pMaskMsg, void *pPatternMsg, void *pFlowControlMsg, unsigned long *pFilterID)
{
    channel_t *ch = chan_from_id(ChannelID);
    PASSTHRU_MSG *mask = (PASSTHRU_MSG *)pMaskMsg;
    PASSTHRU_MSG *patt = (PASSTHRU_MSG *)pPatternMsg;
    PASSTHRU_MSG *fc   = (PASSTHRU_MSG *)pFlowControlMsg;
    int i;
    if (!ch) return ERR_INVALID_CHANNEL_ID;
    if (!mask || !patt || !pFilterID) return ERR_NULL_PARAMETER;
    if (FilterType != PASS_FILTER && FilterType != BLOCK_FILTER && FilterType != FLOW_CONTROL_FILTER)
        return ERR_NOT_SUPPORTED;
    if (FilterType == FLOW_CONTROL_FILTER && !fc) return ERR_NULL_PARAMETER;

    EnterCriticalSection(&ch->filt_lock);
    for (i = 0; i < MAX_FILTERS; i++) if (!ch->filters[i].used) break;
    if (i == MAX_FILTERS) { LeaveCriticalSection(&ch->filt_lock); return ERR_EXCEEDED_LIMIT; }

    {
        filter_t *fl = &ch->filters[i];
        filter_t nf;
        memset(&nf, 0, sizeof(nf));
        nf.type = FilterType;
        if (mask->DataSize >= 4)
            nf.mask_id = ((uint32_t)mask->Data[0]<<24)|((uint32_t)mask->Data[1]<<16)|((uint32_t)mask->Data[2]<<8)|mask->Data[3];
        if (patt->DataSize >= 4)
            nf.patt_id = ((uint32_t)patt->Data[0]<<24)|((uint32_t)patt->Data[1]<<16)|((uint32_t)patt->Data[2]<<8)|patt->Data[3];
        if (FilterType == FLOW_CONTROL_FILTER && fc->DataSize >= 4) {
            nf.fc_id = ((uint32_t)fc->Data[0]<<24)|((uint32_t)fc->Data[1]<<16)|((uint32_t)fc->Data[2]<<8)|fc->Data[3];
            nf.fc_ext = (fc->TxFlags & CAN_29BIT_ID) ? 1 : (nf.fc_id > 0x7FF);
        }
        /* a zero mask means "match everything" is NOT intended for FC; default full mask if app sent 0 for FC */
        if (nf.mask_id == 0 && FilterType == FLOW_CONTROL_FILTER)
            nf.mask_id = nf.patt_id > 0x7FF ? 0x1FFFFFFF : 0x7FF;
        /* Publish the fully-built filter BEFORE marking the slot used. The RX thread reads
         * filters[] unlocked; it must never observe used=1 with mask_id=0 (which matches
         * every frame) during the window between claiming the slot and filling it. */
        nf.used = 0;
        *fl = nf;
        MemoryBarrier();
        fl->used = 1;
    }
    LeaveCriticalSection(&ch->filt_lock);
    *pFilterID = (unsigned long)(i + 1);
    return STATUS_NOERROR;
}

__declspec(dllexport) long __stdcall PassThruStopMsgFilter(unsigned long ChannelID, unsigned long FilterID)
{
    channel_t *ch = chan_from_id(ChannelID);
    if (!ch) return ERR_INVALID_CHANNEL_ID;
    if (FilterID < 1 || FilterID > MAX_FILTERS) return ERR_INVALID_MSG_ID;
    EnterCriticalSection(&ch->filt_lock);
    ch->filters[FilterID-1].used = 0;
    LeaveCriticalSection(&ch->filt_lock);
    return STATUS_NOERROR;
}

__declspec(dllexport) long __stdcall PassThruStartPeriodicMsg(
    unsigned long ChannelID, void *pMsg, unsigned long *pMsgID, unsigned long TimeInterval)
{
    /* v1: not implemented (tester-present can be sent by the app). Return gracefully. */
    (void)pMsg; (void)TimeInterval;
    if (!chan_from_id(ChannelID)) return ERR_INVALID_CHANNEL_ID;
    if (pMsgID) *pMsgID = 1;
    return ERR_NOT_SUPPORTED;
}

__declspec(dllexport) long __stdcall PassThruStopPeriodicMsg(unsigned long ChannelID, unsigned long MsgID)
{
    (void)MsgID;
    if (!chan_from_id(ChannelID)) return ERR_INVALID_CHANNEL_ID;
    return STATUS_NOERROR;
}

__declspec(dllexport) long __stdcall PassThruSetProgrammingVoltage(
    unsigned long DeviceID, unsigned long PinNumber, unsigned long Voltage)
{
    /* No programmable voltage over SocketCAN (FEPS etc. is physical). */
    (void)DeviceID; (void)PinNumber; (void)Voltage;
    return ERR_NOT_SUPPORTED;
}

__declspec(dllexport) long __stdcall PassThruReadVersion(
    unsigned long DeviceID, char *pFirmware, char *pDll, char *pApi)
{
    (void)DeviceID;
    if (!pFirmware || !pDll || !pApi) return ERR_NULL_PARAMETER;
    strcpy(pFirmware, "socketcand bridge");
    strcpy(pDll, "TesPassThruSocketCAN 1.0");
    strcpy(pApi, "04.04");
    return STATUS_NOERROR;
}

__declspec(dllexport) long __stdcall PassThruGetLastError(char *pErrorDescription)
{
    if (!pErrorDescription) return ERR_NULL_PARAMETER;
    strncpy(pErrorDescription, g_last_error, 80); pErrorDescription[79] = 0;
    return STATUS_NOERROR;
}

__declspec(dllexport) long __stdcall PassThruIoctl(
    unsigned long ChannelID, unsigned long IoctlID, void *pInput, void *pOutput)
{
    channel_t *ch = chan_from_id(ChannelID);

    switch (IoctlID) {
        case READ_VBATT:
            if (pOutput) *(unsigned long *)pOutput = 12000;  /* mV — no real VBATT over the network */
            return STATUS_NOERROR;

        case CLEAR_RX_BUFFER:
            if (ch) { EnterCriticalSection(&ch->rq_lock); ch->rq_count = ch->rq_head = ch->rq_tail = 0; LeaveCriticalSection(&ch->rq_lock); }
            return STATUS_NOERROR;

        case CLEAR_TX_BUFFER:
            return STATUS_NOERROR;

        case CLEAR_MSG_FILTERS:
            if (ch) { EnterCriticalSection(&ch->filt_lock); memset(ch->filters, 0, sizeof(ch->filters)); LeaveCriticalSection(&ch->filt_lock); }
            return STATUS_NOERROR;

        case SET_CONFIG: {
            SCONFIG_LIST *lst = (SCONFIG_LIST *)pInput;
            unsigned j;
            if (!lst || !lst->ConfigPtr) return STATUS_NOERROR;
            for (j = 0; j < lst->NumOfParams; j++) {
                if (ch && lst->ConfigPtr[j].Parameter == LOOPBACK) ch->loopback = (int)lst->ConfigPtr[j].Value;
            }
            return STATUS_NOERROR;
        }
        case GET_CONFIG: {
            SCONFIG_LIST *lst = (SCONFIG_LIST *)pInput;
            unsigned j;
            if (!lst || !lst->ConfigPtr) return STATUS_NOERROR;
            for (j = 0; j < lst->NumOfParams; j++) {
                if (lst->ConfigPtr[j].Parameter == DATA_RATE) lst->ConfigPtr[j].Value = ch ? ch->baud : g_bitrate;
                else if (lst->ConfigPtr[j].Parameter == LOOPBACK) lst->ConfigPtr[j].Value = ch ? ch->loopback : 0;
            }
            return STATUS_NOERROR;
        }
        default:
            return STATUS_NOERROR;   /* tolerate unknown ioctls rather than failing the session */
    }
}
