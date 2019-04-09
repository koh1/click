// Generated by make-ip-conf.pl
// enp0s3 10.0.2.15 02:48:3e:15:b5:0c
// enp0s8 172.16.101.11 08:00:27:de:92:4c
// enp0s9 172.16.102.11 08:00:27:36:70:32
// enp0s10 172.16.103.11 08:00:27:86:96:5a
// enp0s16 172.16.108.11 08:00:27:d7:6e:7b

// Shared IP input path and routing table
//ip :: CheckIPHeader(INTERFACES 10.0.2.15/255.255.255.0 172.16.101.11/255.255.255.0 172.16.102.11/255.255.255.0 172.16.103.11/255.255.255.0 172.16.108.11/255.255.255.0)

rt :: StaticIPLookup(
   	10.0.2.15/32 0,
	10.0.2.255/32 0,
	10.0.2.0/32 0,
	172.16.101.11/32 0,
	172.16.101.255/32 0,
	172.16.101.0/32 0,
	172.16.102.11/32 0,
	172.16.102.255/32 0,
	172.16.102.0/32 0,
	172.16.103.11/32 0,
	172.16.103.255/32 0,
	172.16.103.0/32 0,
	172.16.108.11/32 0,
	172.16.108.255/32 0,
	172.16.108.0/32 0,
	10.0.2.0/255.255.255.0 1,
	172.16.101.0/255.255.255.0 2,
	172.16.102.0/255.255.255.0 3,
	172.16.103.0/255.255.255.0 4,
	172.16.108.0/255.255.255.0 5,
	255.255.255.255/32 0.0.0.0 0,
	0.0.0.0/32 0,
	172.16.104.0/255.255.255.0 172.16.102.12 3,
	172.16.105.0/255.255.255.0 172.16.103.13 4,
	172.16.106.0/255.255.255.0 172.16.102.12 3,
	172.16.107.0/255.255.255.0 172.16.103.13 4,
	172.16.109.0/255.255.255.0 172.16.102.12 3,
	172.16.110.0/255.255.255.0 172.16.103.13 4,
	0.0.0.0/0.0.0.0 10.0.2.2 1);

// ARP responses are copied to each ARPQuerier and the host.
//arpt :: Tee(6);

// Input and output paths for enp0s3
//c0 :: Classifier(12/0806 20/0001, 12/0806 20/0002, 12/0800, -);
//FromDevice(enp0s3) -> c0;
//out0 :: Queue(1024) -> todevice0 :: ToDevice(enp0s3);
//c0[0] -> ar0 :: ARPResponder(10.0.2.15 02:48:3e:15:b5:0c) -> out0;
//arpq0 :: ARPQuerier(10.0.2.15, 02:48:3e:15:b5:0c) -> out0;
//c0[1] -> [1]arpq0;
//c0[2] -> Paint(1) -> ip;
//c0[3] -> Print("enp0s3 non-IP") -> Discard;

// Input and output paths for enp0s8
c1 :: Classifier(12/0806 20/0001, 12/0806 20/0002, 12/0800, -);
FromDevice(enp0s8) -> c1;
out1 :: Queue(1024) -> todevice1 :: ToDevice(enp0s8);
c1[0] -> ar1 :: ARPResponder(172.16.101.11 08:00:27:de:92:4c) -> out1;
arpq1 :: ARPQuerier(172.16.101.11, 08:00:27:de:92:4c) -> out1;
c1[1] -> [1]arpq1;
//c1[2] -> Paint(2) -> ip;
c1[2]
	-> Strip(14)
	-> CheckIPHeader()
	-> udpip_cl1 :: IPClassifier(udp port 33333, -)
	-> Strip(28)
	-> DDBPrint
	-> ddb_cl1 :: DDBClassifier
	-> ddb_ans1 :: DDBAnswer
	-> IPPrint(RESP08)	
	-> [0]rt;
c1[3] -> Print("enp0s8 non-IP") -> Discard;

// Input and output paths for enp0s9
c2 :: Classifier(12/0806 20/0001, 12/0806 20/0002, 12/0800, -);
FromDevice(enp0s9) -> c2;
out2 :: Queue(1024) -> todevice2 :: ToDevice(enp0s9);
c2[0] -> ar2 :: ARPResponder(172.16.102.11 08:00:27:36:70:32) -> out2;
arpq2 :: ARPQuerier(172.16.102.11, 08:00:27:36:70:32) -> out2;
c2[1] -> [1]arpq2;
//c2[2] -> Paint(3) -> ip;
c2[2]
	-> Strip(14)
	-> CheckIPHeader()
	-> udpip_cl2 :: IPClassifier(dst udp port 33333, dst udp port 33332, -)
	-> Strip(28)
	-> DDBPrint
	-> ddb_cl2 :: DDBClassifier
	-> ddb_ans2 :: DDBAnswer
	-> IPPrint(RESP09)	
	-> [0]rt;
c2[3] -> Print("enp0s9 non-IP") -> Discard;

// Input and output paths for enp0s10
c3 :: Classifier(12/0806 20/0001, 12/0806 20/0002, 12/0800, -);
FromDevice(enp0s10) -> c3;
out3 :: Queue(1024) -> todevice3 :: ToDevice(enp0s10);
c3[0] -> ar3 :: ARPResponder(172.16.103.11 08:00:27:86:96:5a) -> out3;
arpq3 :: ARPQuerier(172.16.103.11, 08:00:27:86:96:5a) -> out3;
c3[1] -> [1]arpq3;
//c3[2] -> Paint(4) -> ip;
c3[2]
	-> Strip(14)
	-> CheckIPHeader()
	-> udpip_cl3 :: IPClassifier(udp port 33333, -)
	-> Strip(28)
	-> DDBPrint
	-> ddb_cl3 :: DDBClassifier
	-> ddb_ans3 :: DDBAnswer
	-> IPPrint(RESP10)
	-> [0]rt;
c3[3] -> Print("enp0s10 non-IP") -> Discard;

// Input and output paths for enp0s16
c4 :: Classifier(12/0806 20/0001, 12/0806 20/0002, 12/0800, -);
FromDevice(enp0s16) -> c4;
out4 :: Queue(200) -> todevice4 :: ToDevice(enp0s16);
c4[0] -> ar4 :: ARPResponder(172.16.108.11 08:00:27:d7:6e:7b) -> out4;
arpq4 :: ARPQuerier(172.16.108.11, 08:00:27:d7:6e:7b) -> out4;
c4[1] -> [1]arpq4;
//c4[2] -> Paint(5) -> ip;
c4[2]
	-> Strip(14)
	-> CheckIPHeader()
	-> udpip_cl4 :: IPClassifier(udp port 33333, -)
	-> Strip(28)
	-> DDBPrint
	-> ddb_cl4 :: DDBClassifier
	-> ddb_ans4 :: DDBAnswer
	-> IPPrint(RESP16)
	-> [0]rt;
c4[3] -> Print("enp0s16 non-IP") -> Discard;

udpip_cl1[1] -> IPPrint(IP1) -> [0]rt;
udpip_cl2[1]
	-> Strip(28)
	-> DDBPrint
	-> DDBRequest
	-> DDBPrint
	-> Discard;
udpip_cl2[2] -> IPPrint(IP2) -> [0]rt;
udpip_cl3[1] -> IPPrint(IP3) -> [0]rt;
udpip_cl4[1] -> IPPrint(IP4) -> [0]rt;

ddb_cl1[1] -> DDBLog; ddb_cl2[1] -> DDBLog; ddb_cl3[1] -> DDBLog; ddb_cl4[1] -> DDBLog;
ddb_cl1[2] -> DDBLog; ddb_cl2[2] -> DDBLog; ddb_cl3[2] -> DDBLog; ddb_cl4[2] -> DDBLog;
ddb_cl1[3] -> DDBLog; ddb_cl2[3] -> DDBLog; ddb_cl3[3] -> DDBLog; ddb_cl4[3] -> DDBLog;
ddb_cl1[4] -> Discard; ddb_cl2[4] -> Discard; ddb_cl3[4] -> Discard; ddb_cl4[4] -> Discard;

ddb_ans1[1] -> IPPrint(MISHIT1) -> [0]rt;
ddb_ans2[1] -> IPPrint(MISHIT2) -> [0]rt;
ddb_ans3[1] -> IPPrint(MISHIT3) -> [0]rt;
ddb_ans4[1] -> IPPrint(MISHIT4) -> [0]rt;

// Local delivery
toh :: Print(toh) -> Discard;
//arpt[5] -> toh;
//rt[0] -> EtherEncap(0x0800, 1:1:1:1:1:1, 2:2:2:2:2:2) -> toh;
rt[0] -> toh;

// Forwarding path for enp0s3
rt[1] -> Discard;
//rt[1] -> DropBroadcasts
//    -> cp0 :: PaintTee(1)
//    -> gio0 :: IPGWOptions(10.0.2.15)
//    -> FixIPSrc(10.0.2.15)
//    -> dt0 :: DecIPTTL
//    -> fr0 :: IPFragmenter(1400)
//    -> [0]arpq0;
//dt0[1] -> ICMPError(10.0.2.15, timeexceeded) -> rt;
//fr0[1] -> ICMPError(10.0.2.15, unreachable, needfrag) -> rt;
//gio0[1] -> ICMPError(10.0.2.15, parameterproblem) -> rt;
//cp0[1] -> ICMPError(10.0.2.15, redirect, host) -> rt;

// Forwarding path for enp0s8
rt[2] -> DropBroadcasts
    -> gio1 :: IPGWOptions(172.16.101.11)
    -> dt1 :: DecIPTTL
    -> fr1 :: IPFragmenter(1400)
    -> IPPrint(RT2)
    -> [0]arpq1;
dt1[1] -> ICMPError(172.16.101.11, timeexceeded) -> [0]rt;
fr1[1] -> ICMPError(172.16.101.11, unreachable, needfrag) -> [0]rt;
gio1[1] -> ICMPError(172.16.101.11, parameterproblem) -> [0]rt;

// Forwarding path for enp0s9
rt[3] -> DropBroadcasts
    -> gio2 :: IPGWOptions(172.16.102.11)
    -> dt2 :: DecIPTTL
    -> fr2 :: IPFragmenter(1400)
    -> IPPrint(RT3)
    -> [0]arpq2;
dt2[1] -> ICMPError(172.16.102.11, timeexceeded) -> [0]rt;
fr2[1] -> ICMPError(172.16.102.11, unreachable, needfrag) -> [0]rt;
gio2[1] -> ICMPError(172.16.102.11, parameterproblem) -> [0]rt;

// Forwarding path for enp0s10
rt[4] -> DropBroadcasts
    -> gio3 :: IPGWOptions(172.16.103.11)
    -> dt3 :: DecIPTTL
    -> fr3 :: IPFragmenter(1400)
    -> IPPrint(RT4)
    -> [0]arpq3;
dt3[1] -> ICMPError(172.16.103.11, timeexceeded) -> [0]rt;
fr3[1] -> ICMPError(172.16.103.11, unreachable, needfrag) -> [0]rt;
gio3[1] -> ICMPError(172.16.103.11, parameterproblem) -> [0]rt;

// Forwarding path for enp0s16
rt[5] -> DropBroadcasts
    -> gio4 :: IPGWOptions(172.16.108.11)
    -> dt4 :: DecIPTTL
    -> fr4 :: IPFragmenter(1400)
    -> IPPrint(RT5)
    -> [0]arpq4;
dt4[1] -> ICMPError(172.16.108.11, timeexceeded) -> [0]rt;
fr4[1] -> ICMPError(172.16.108.11, unreachable, needfrag) -> [0]rt;
gio4[1] -> ICMPError(172.16.108.11, parameterproblem) -> [0]rt;