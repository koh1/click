// Generated by make-ip-conf.pl
// enp0s3 10.0.2.15 02:12:4b:e9:82:8c
// enp0s8 192.168.148.70 08:00:27:c2:73:e7
// enp0s9 192.168.147.70 08:00:27:63:4f:c6

// Shared IP input path and routing table

rt :: StaticIPLookup(
   	10.0.2.15/32 0,
	10.0.2.255/32 0,
//	192.168.148.70/32 0,
//	192.168.147.70/32 0,
	192.168.148.255/32 0,
	192.168.147.255/32 0,
	10.0.2.0/255.255.255.0 1,
	192.168.148.0/24 2,
	192.168.147.0/24 3,
	255.255.255.255/32 0.0.0.0 0,
//	0.0.0.0/32 0,
	0.0.0.0/0.0.0.0 192.168.148.1 2);

// ARP responses are copied to each ARPQuerier and the host.
//arpt :: Tee(6);

// Input and output paths for enp0s3
c0 :: Classifier(12/0806 20/0001, 12/0806 20/0002, 12/0800, -);
FromDevice(enp0s3) -> c0;
out0 :: Queue(1024) -> todevice0 :: ToDevice(enp0s3);
c0[0] -> ar0 :: ARPResponder(10.0.2.15 02:48:3e:15:b5:0c) -> out0;
arpq0 :: ARPQuerier(10.0.2.15, 02:48:3e:15:b5:0c) -> out0;
c0[1] -> [1]arpq0;
c0[2] -> Paint(1) -> Discard;
//c0[3] -> Print("enp0s3 non-IP") -> Discard;
c0[3] -> Discard

// Input and output paths for enp0s8

// [0]ARP Query, [1]ARP Response, [2]IP, [3]Non-IP
c1 :: Classifier(12/0806 20/0001, 12/0806 20/0002, 12/0800, -);
FromDevice(enp0s8) -> c1;
out1 :: Queue(1024) -> todevice1 :: ToDevice(enp0s8);
c1[0] -> ar1 :: ARPResponder(192.168.148.70 08:00:27:c2:73:e7) -> out1;
arpq1 :: ARPQuerier(192.168.148.70, 08:00:27:c2:73:e7) -> out1;
c1[1] -> [1]arpq1;
c1[2] -> [0]rt;
//c1[3] -> Print("enp0s8 non-IP") -> Discard;
c1[3] -> Discard


// Input and output paths for enp0s9
c2 :: Classifier(12/0806 20/0001, 12/0806 20/0002, 12/0800, -);
FromDevice(enp0s9) -> c2;
out2 :: Queue(1024) -> todevice2 :: ToDevice(enp0s9);
c2[0] -> ar2 :: ARPResponder(192.168.147.11 08:00:27:36:70:32) -> out2;
arpq2 :: ARPQuerier(192.168.147.11, 08:00:27:36:70:32) -> out2;
c2[1] -> [1]arpq2;
c2[2] -> [0]rt;
//c2[3] -> Print("enp0s9 non-IP") -> Discard;
c2[3] -> Discard


// Local delivery
//toh :: Print(toh) -> Discard;
rt[0] -> Discard;

// Forwarding path for enp0s3

rt[1] -> DropBroadcasts
//    -> cp0 :: PaintTee(1)
    -> gio0 :: IPGWOptions(10.0.2.15)
//    -> FixIPSrc(10.0.2.15)
    -> dt0 :: DecIPTTL
    -> fr0 :: IPFragmenter(1500)
    -> [0]arpq0;
dt0[1] -> ICMPError(10.0.2.15, timeexceeded) -> [0]rt;
fr0[1] -> ICMPError(10.0.2.15, unreachable, needfrag) -> [0]rt;
gio0[1] -> ICMPError(10.0.2.15, parameterproblem) -> [0]rt;
//cp0[1] -> ICMPError(10.0.2.15, redirect, host) -> [0]rt;

// Forwarding path for enp0s8
rt[2] -> DropBroadcasts
//    -> IPPrint("enp0s8-IP")
    -> gio1 :: IPGWOptions(192.168.148.70)
    -> dt1 :: DecIPTTL
    -> fr1 :: IPFragmenter(1500)
    -> IPPrint(RT2)
    -> [0]arpq1;
dt1[1] -> ICMPError(192.168.148.70, timeexceeded) -> [0]rt;
fr1[1] -> ICMPError(192.168.148.70, unreachable, needfrag) -> [0]rt;
gio1[1] -> ICMPError(192.168.148.70, parameterproblem) -> [0]rt;


// Forwarding path for enp0s9
rt[3] -> DropBroadcasts
//    -> IPPrint("enp0s9-IP")
    -> gio2 :: IPGWOptions(192.168.147.70)
    -> dt2 :: DecIPTTL
    -> fr2 :: IPFragmenter(1500)
    -> IPPrint(RT3)
    -> [0]arpq2;
dt2[1] -> ICMPError(192.168.147.70, timeexceeded) -> [0]rt;
fr2[1] -> ICMPError(192.168.147.70, unreachable, needfrag) -> [0]rt;
gio2[1] -> ICMPError(192.168.147.70, parameterproblem) -> [0]rt;

