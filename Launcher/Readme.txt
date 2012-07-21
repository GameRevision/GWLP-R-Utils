Use this tool to connect to GWLP servers without modifying the HOST file.

HOW TO USE:

1. Copy all files (GWLP Launcher.exe,GWLP.dll,GWLP.xml) to your GW main folder.

2. Edit GWLP.xml to match your GWLP server.

Pattern:

<EncryptConnection>1 or 0</EncryptConnection>

<MultiClient>
	<Enable>1 or 0</Enable>
	<UnlockGwDat>1 or 0</UnlockGwDat>
</MultiClient>

<FileServer Id = X>
	<Hostname>H</Hostname> or <IP>I</IP>
	<Port>P</Port>
</FileServer>
....

<LoginServer>
	<Hostname>H</Hostname> or <IP>I</IP>
	<Port>P</Port>
</LoginServer>

where X = the FileServers Id (1-12). Note that you have to Provide Hostnames/IPs for 12 File Servers.
      H = the DNS of your GWLP server (eg. gwlp.com). You can specify different Hostnames for all Servers.
      I = the IP of your GWLP server (eg. 127.0.0.1)
      P = the Port your GWLP server is running on (eg. 6112)

3. Start GWLP Launcher.exe. Note that all Commandline Arguments you are passing to GWLP Launcher will be passed to GW.
	eg GWLP Launcher.exe -nosound will start GW with -nosound enabled.



Please report all Bugs.

© ACB