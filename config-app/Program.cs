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
// TesSocketCANConfig - setup & configuration for the SocketCAN J2534 PassThru DLL.
// .NET Framework 4.8.1 WinForms, default theme. Writes the J2534 registration (HKLM, 32-bit view)
// and the DLL's target config (host/port/interface/bitrate/listen), and tests the socketcand link.
//
// Tester Present Specialist Automotive Solutions - https://github.com/jakka351/PassThru-SocketCAN
using System;
using System.Drawing;
using System.IO;
using System.Net.Sockets;
using System.Text;
using System.Windows.Forms;
using Microsoft.Win32;

namespace TesSocketCANConfig
{
    internal static class Program
    {
        [STAThread]
        private static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MainForm());
        }
    }

    public sealed class MainForm : Form
    {
        // Registry locations (written/read in the 32-bit view -> WOW6432Node on 64-bit Windows,
        // because the DLL and the J2534 apps that enumerate it are 32-bit).
        private const string J2534Key   = @"SOFTWARE\PassThruSupport.04.04\Tester Present SocketCAN";
        private const string ConfigKey  = @"SOFTWARE\Tester Present\PassThru-SocketCAN";
        private const string DeviceName = "Tester Present SocketCAN";
        private const string Vendor     = "Tester Present Specialist Automotive Solutions";

        private readonly TextBox _host  = new TextBox();
        private readonly TextBox _port  = new TextBox();
        private readonly TextBox _iface = new TextBox();
        private readonly ComboBox _baud = new ComboBox();
        private readonly CheckBox _listen = new CheckBox();
        private readonly TextBox _dll   = new TextBox();
        private readonly TextBox _log   = new TextBox();

        public MainForm()
        {
            Text = "Tester Present - SocketCAN J2534 Setup";
            FormBorderStyle = FormBorderStyle.FixedDialog;
            MaximizeBox = false;
            StartPosition = FormStartPosition.CenterScreen;
            ClientSize = new Size(560, 470);
            Font = new Font("Segoe UI", 9f);

            int y = 14;
            AddLabel("Raspberry Pi host / IP:", 16, y);
            _host.SetBounds(220, y - 3, 320, 24); Controls.Add(_host);
            y += 34;
            AddLabel("socketcand TCP port:", 16, y);
            _port.SetBounds(220, y - 3, 100, 24); Controls.Add(_port);
            y += 34;
            AddLabel("SocketCAN interface:", 16, y);
            _iface.SetBounds(220, y - 3, 100, 24); Controls.Add(_iface);
            y += 34;
            AddLabel("CAN bus speed (bit/s):", 16, y);
            _baud.SetBounds(220, y - 3, 160, 24);
            _baud.DropDownStyle = ComboBoxStyle.DropDownList;
            _baud.Items.AddRange(new object[] { "500000  (HS-CAN / OBD-II)", "250000  (J1939)", "125000  (MS-CAN)", "1000000  (1 Mbit)" });
            Controls.Add(_baud);
            y += 34;
            _listen.SetBounds(220, y - 2, 320, 22);
            _listen.Text = "Listen-only (never transmit / ACK)";
            Controls.Add(_listen);
            y += 34;

            AddLabel("J2534 DLL path:", 16, y);
            _dll.SetBounds(220, y - 3, 240, 24); Controls.Add(_dll);
            var browse = new Button { Text = "Browse...", Left = 466, Top = y - 4, Width = 74, Height = 26 };
            browse.Click += (s, e) => BrowseDll();
            Controls.Add(browse);
            y += 44;

            var bTest = MakeButton("Test Connection", 16, y, 128);
            bTest.Click += (s, e) => TestConnection();
            var bSave = MakeButton("Save Config", 152, y, 110);
            bSave.Click += (s, e) => { if (SaveConfig()) LogLine("Config saved."); };
            var bReg = MakeButton("Register Device", 270, y, 128);
            bReg.Click += (s, e) => RegisterDevice();
            var bUnreg = MakeButton("Unregister", 406, y, 100);
            bUnreg.Click += (s, e) => UnregisterDevice();
            y += 40;

            _log.SetBounds(16, y, 524, 470 - y - 14);
            _log.Multiline = true; _log.ReadOnly = true; _log.ScrollBars = ScrollBars.Vertical;
            _log.BackColor = SystemColors.Window; _log.Font = new Font("Consolas", 8.5f);
            Controls.Add(_log);

            LoadConfig();
        }

        private void AddLabel(string text, int x, int y)
        {
            Controls.Add(new Label { Text = text, Left = x, Top = y, Width = 200, Height = 20 });
        }

        private Button MakeButton(string text, int x, int y, int w)
        {
            var b = new Button { Text = text, Left = x, Top = y, Width = w, Height = 30 };
            Controls.Add(b);
            return b;
        }

        private void LogLine(string s) { _log.AppendText(s + Environment.NewLine); }

        private uint SelectedBaud()
        {
            switch (_baud.SelectedIndex)
            {
                case 1: return 250000;
                case 2: return 125000;
                case 3: return 1000000;
                default: return 500000;
            }
        }

        private void SetBaudIndex(uint b)
        {
            _baud.SelectedIndex = b == 250000 ? 1 : b == 125000 ? 2 : b == 1000000 ? 3 : 0;
        }

        private static RegistryKey HklmBase() =>
            RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry32);

        // ---- config load/save ----------------------------------------------------------
        private void LoadConfig()
        {
            string host = "10.42.0.1", iface = "can0", dll = DefaultDllPath();
            int port = 29536, listen = 0; uint baud = 500000;
            try
            {
                using (var b = HklmBase())
                using (var k = b.OpenSubKey(ConfigKey))
                {
                    if (k != null)
                    {
                        host  = (string)(k.GetValue("Host") ?? host);
                        iface = (string)(k.GetValue("Interface") ?? iface);
                        port  = (int)(k.GetValue("Port") ?? port);
                        baud  = (uint)(int)(k.GetValue("Bitrate") ?? (int)baud);
                        listen = (int)(k.GetValue("ListenOnly") ?? 0);
                    }
                }
                using (var b = HklmBase())
                using (var k = b.OpenSubKey(J2534Key))
                {
                    if (k != null) dll = (string)(k.GetValue("FunctionLibrary") ?? dll);
                }
            }
            catch { /* first run — defaults */ }

            _host.Text = host; _port.Text = port.ToString(); _iface.Text = iface;
            SetBaudIndex(baud); _listen.Checked = listen != 0; _dll.Text = dll;
            LogLine("Loaded. Fill in the Pi details, Test Connection, then Register Device.");
        }

        private string DefaultDllPath()
        {
            // Prefer a DLL sitting next to this exe; else the sibling dll\ build output.
            string exeDir = AppDomain.CurrentDomain.BaseDirectory;
            string local = Path.Combine(exeDir, "TesPassThruSocketCAN.dll");
            if (File.Exists(local)) return local;
            try
            {
                string guess = Path.GetFullPath(Path.Combine(exeDir, @"..\..\..\dll\TesPassThruSocketCAN.dll"));
                if (File.Exists(guess)) return guess;
            }
            catch { }
            return local;
        }

        private bool SaveConfig()
        {
            if (!int.TryParse(_port.Text.Trim(), out int port) || port <= 0) { Warn("Port must be a positive number."); return false; }
            if (string.IsNullOrWhiteSpace(_host.Text)) { Warn("Enter the Pi host / IP."); return false; }
            if (string.IsNullOrWhiteSpace(_iface.Text)) { Warn("Enter the SocketCAN interface (e.g. can0)."); return false; }
            try
            {
                using (var b = HklmBase())
                using (var k = b.CreateSubKey(ConfigKey))
                {
                    k.SetValue("Host", _host.Text.Trim(), RegistryValueKind.String);
                    k.SetValue("Port", port, RegistryValueKind.DWord);
                    k.SetValue("Interface", _iface.Text.Trim(), RegistryValueKind.String);
                    k.SetValue("Bitrate", (int)SelectedBaud(), RegistryValueKind.DWord);
                    k.SetValue("ListenOnly", _listen.Checked ? 1 : 0, RegistryValueKind.DWord);
                }
                return true;
            }
            catch (Exception ex) { Warn("Could not write config (run as administrator?): " + ex.Message); return false; }
        }

        // ---- J2534 registration ---------------------------------------------------------
        private void RegisterDevice()
        {
            string dll = _dll.Text.Trim();
            if (!File.Exists(dll)) { Warn("DLL not found:\n" + dll + "\n\nBuild it first (dll\\build.bat) or Browse to it."); return; }
            if (!SaveConfig()) return;
            try
            {
                using (var b = HklmBase())
                using (var k = b.CreateSubKey(J2534Key))
                {
                    k.SetValue("Name", DeviceName, RegistryValueKind.String);
                    k.SetValue("Vendor", Vendor, RegistryValueKind.String);
                    k.SetValue("FunctionLibrary", dll, RegistryValueKind.String);
                    k.SetValue("ConfigApplication", Application.ExecutablePath, RegistryValueKind.String);
                    // Protocols this DLL supports (1 = supported).
                    k.SetValue("CAN", 1, RegistryValueKind.DWord);
                    k.SetValue("ISO15765", 1, RegistryValueKind.DWord);
                    // Protocols we explicitly do NOT support over SocketCAN.
                    k.SetValue("ISO9141", 0, RegistryValueKind.DWord);
                    k.SetValue("ISO14230", 0, RegistryValueKind.DWord);
                    k.SetValue("J1850PWM", 0, RegistryValueKind.DWord);
                    k.SetValue("J1850VPW", 0, RegistryValueKind.DWord);
                }
                LogLine("Registered '" + DeviceName + "' as a J2534 device.");
                LogLine("  FunctionLibrary = " + dll);
                LogLine("J2534 apps (TES, ForScan, IDS, HP Tuners...) will now list it.");
                MessageBox.Show(this, "'" + DeviceName + "' is registered as a J2534 PassThru device.\n\n" +
                    "It will appear in the device list of any J2534 application.", "Registered",
                    MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex) { Warn("Registration failed (run as administrator?): " + ex.Message); }
        }

        private void UnregisterDevice()
        {
            try
            {
                using (var b = HklmBase())
                    b.DeleteSubKeyTree(J2534Key, throwOnMissingSubKey: false);
                LogLine("Unregistered '" + DeviceName + "'.");
                MessageBox.Show(this, "Device unregistered.", "Unregistered", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex) { Warn("Unregister failed (run as administrator?): " + ex.Message); }
        }

        // ---- socketcand connection test -------------------------------------------------
        private void TestConnection()
        {
            if (!int.TryParse(_port.Text.Trim(), out int port) || port <= 0) { Warn("Port must be a positive number."); return; }
            string host = _host.Text.Trim(), iface = _iface.Text.Trim();
            LogLine(string.Format("Testing socketcand at {0}:{1}, interface {2} ...", host, port, iface));
            try
            {
                using (var client = new TcpClient { NoDelay = true })
                {
                    var ar = client.BeginConnect(host, port, null, null);
                    if (!ar.AsyncWaitHandle.WaitOne(TimeSpan.FromSeconds(5)) || !client.Connected)
                    { LogLine("  FAILED: could not reach socketcand (timeout). Is it running and the port open?"); return; }
                    client.EndConnect(ar);
                    using (var ns = client.GetStream())
                    {
                        ns.ReadTimeout = 2000;
                        ReadMsg(ns);                                   // tolerate "< hi >"
                        if (!Command(ns, "< open " + iface + " >")) { LogLine("  FAILED: 'open " + iface + "' not accepted. Wrong interface name?"); return; }
                        if (!Command(ns, "< rawmode >"))            { LogLine("  FAILED: 'rawmode' not accepted."); return; }
                    }
                }
                LogLine("  SUCCESS - socketcand reachable and the interface opened in raw mode.");
                MessageBox.Show(this, "Connected to socketcand and opened " + iface + " in raw mode.", "Connection OK",
                    MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex) { LogLine("  FAILED: " + ex.Message); }
        }

        private static bool Command(NetworkStream ns, string cmd)
        {
            byte[] b = Encoding.ASCII.GetBytes(cmd);
            ns.Write(b, 0, b.Length); ns.Flush();
            string reply = ReadMsg(ns);
            return reply != null && reply.IndexOf("ok", StringComparison.OrdinalIgnoreCase) >= 0;
        }

        private static string ReadMsg(NetworkStream ns)
        {
            try
            {
                var sb = new StringBuilder();
                int c; bool started = false;
                while ((c = ns.ReadByte()) >= 0)
                {
                    char ch = (char)c;
                    if (!started) { if (ch == '<') started = true; continue; }
                    if (ch == '>') return sb.ToString().Trim();
                    sb.Append(ch);
                }
            }
            catch { }
            return null;
        }

        private void BrowseDll()
        {
            using (var ofd = new OpenFileDialog { Filter = "J2534 DLL (*.dll)|*.dll", Title = "Select TesPassThruSocketCAN.dll" })
            {
                if (File.Exists(_dll.Text)) ofd.FileName = _dll.Text;
                if (ofd.ShowDialog(this) == DialogResult.OK) _dll.Text = ofd.FileName;
            }
        }

        private void Warn(string s) { LogLine("  " + s); MessageBox.Show(this, s, "Tester Present SocketCAN", MessageBoxButtons.OK, MessageBoxIcon.Warning); }
    }
}
