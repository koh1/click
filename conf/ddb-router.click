FromHost(enp0s8, 172.16.101.11/32)
	-> fromhost_c08 :: Classifier(
	   12/0800,    // IP
	   12/0806 20/0001,	// ARP Queries
	   -);
FromHost(enp0s9, 172.16.102.11/32)
	-> fromhost_c09 :: Classifier(
	   12/0800,    // IP
	   12/0806 20/0001,	// ARP Queries
	   -);
FromHost(enp0s10, 172.16.103.11/32)
	-> fromhost_c10 :: Classifier(
	   12/0800,    // IP
	   12/0806 20/0001,	// ARP Queries
	   -);

FromDevice(enp0s8) -> c8 :: Classifier(
		   12/0806 20/0001,
		   12/0806 20/0002,
		   12/0800,
		   -);
FromDevice(enp0s9) -> c9 :: Classifier(
		   12/0806 20/0001,
		   12/0806 20/0002,
		   12/0800,
		   -);
FromDevice(enp0s10) -> c10 :: Classifier(
		   12/0806 20/0001,
		   12/0806 20/0002,
		   12/0800,
		   -);

ddb_classifier08 :: IPClassifier(dst udp port 33333, -)
ddb_classifier09 :: IPClassifier(dst udp port 33333, -)
ddb_classifier10 :: IPClassifier(dst udp port 33333, -)

ddb_out_classifier08 :: IPClassifier(dst udp port 33333, -)
ddb_out_classifier09 :: IPClassifier(dst udp port 33333, -)
ddb_out_classifier10 :: IPClassifier(dst udp port 33333, -)

arpq08 :: ARPQuerier(172.16.101.11, 08:00:27:de:92:4c);
arpq09 :: ARPQuerier(172.16.102.11, 08:00:27:36:70:32);
arpq10 :: ARPQuerier(172.16.103.11, 08:00:27:86:96:5a);

out08 :: Queue(1024) -> ToDevice(enp0s8);
out09 :: Queue(1024) -> ToDevice(enp0s9);
out10 :: Queue(1024) -> ToDevice(enp0s10);
out03 :: Queue(1024) -> ToDevice(enp0s3);

rt :: StaticIPLookup(
   172.16.101.11/32 0,
   172.16.102.11/32 5,
   172.16.103.11/32 6,
   172.16.101.0/24 1,
   172.16.102.0/24 2,
   172.16.103.0/24 3,
   172.16.104.0/24 172.16.102.12 2,
   172.16.105.0/24 172.16.103.13 3,
   172.16.106.0/24 172.16.102.12 2,
   172.16.107.0/24 172.16.103.13 3,
   0.0.0.0/0 10.0.2.15 4);

iprwddb08 :: IPRewriter(
     pattern 172.16.101.101 - - - 0 0
);

iprwddb09 :: IPRewriter(
     pattern 172.16.101.101 - - - 0 0
);

iprwddb10 :: IPRewriter(
     pattern 172.16.101.101 - - - 0 0
);

iprw08 :: IPRewriter(
     pattern - - 172.16.101.11 - 0 0
);

iprw09 :: IPRewriter(
     pattern - - 172.16.102.11 - 0 0
);
iprw10 :: IPRewriter(
     pattern - - 172.16.103.11 - 0 0
);


// OUTPUT Classifier
fromhost_c08 -> ddb_out_classifier08 -> [0]iprwddb08 -> [0]arpq08;
fromhost_c08[1] -> out08;
fromhost_c08[2] -> out08;
ddb_out_classifier08[1] -> [0]arpq08;

fromhost_c09 -> ddb_out_classifier09 -> [0]iprwddb09 -> [0]arpq09;
fromhost_c09[1] -> out09;
fromhost_c09[2] -> out09;
ddb_out_classifier09[1] -> [0]arpq09;

fromhost_c10 -> ddb_out_classifier10 -> [0]iprwddb10 -> [0]arpq10;
fromhost_c10[1] -> out10;
fromhost_c10[2] -> out10;
ddb_out_classifier10[1] -> [0]arpq10;


// INPUT Classifier
c8[0]	-> ARPResponder(172.16.101.11 08:00:27:de:92:4c) -> out08;
c8[1]	-> [1]arpq08;
c8[2]	-> Strip(14) -> CheckIPHeader()	-> [0]rt;
c8[3]	-> Discard;

c9[0]	-> ARPResponder(172.16.102.11 08:00:27:36:70:32) -> out09;
c9[1]	-> [1]arpq09;
c9[2]	-> Strip(14) -> CheckIPHeader()	-> [0]rt;
c9[3]	-> Discard;

c10[0]	-> ARPResponder(172.16.103.11 08:00:27:86:96:5a) -> out10;
c10[1]	-> [1]arpq10;
c10[2]	-> Strip(14) -> CheckIPHeader()	-> [0]rt;
c10[3]	-> Discard;


// routing definitions
rt[0] -> IPPrint(MY08) -> Discard; //ToHost(eth0);
rt[5] -> IPPrint(MY09) -> Discard; //ToHost(enp0s9);
rt[6] -> IPPrint(MY10) -> Discard; //ToHost(enp0s10);

rt[1] -> ddb_classifier08 -> IPPrint(BEFORE) -> [0]iprw08 -> IPPrint(AFTER) -> Discard;
ddb_classifier08[1]
	-> DropBroadcasts
	-> gio1 :: IPGWOptions(172.16.101.11)
	-> dt1 :: DecIPTTL ->  fr1 :: IPFragmenter(300) -> [0]arpq08;

rt[2] -> ddb_classifier09 -> IPPrint(BEFORE) -> [0]iprw09 -> IPPrint(AFTER) -> Discard;
ddb_classifier09[1]
	-> DropBroadcasts
	-> gio2 :: IPGWOptions(172.16.102.11)
	-> dt2 :: DecIPTTL -> fr2 :: IPFragmenter(300) -> [0]arpq09;

rt[3] -> ddb_classifier10 -> IPPrint(BEFORE) -> [0]iprw10 -> IPPrint(AFTER) -> ToHost(enp0s10);
ddb_classifier10[1]
	-> DropBroadcasts
	-> gio3 :: IPGWOptions(172.16.103.11)
	-> dt3 :: DecIPTTL -> fr3 :: IPFragmenter(300) -> [0]arpq10;

rt[4] -> out03;



dt1[1] -> ICMPError(172.16.101.11, timeexceeded) -> [0]rt;
dt2[1] -> ICMPError(172.16.102.11, timeexceeded) -> [0]rt;
dt3[1] -> ICMPError(172.16.103.11, timeexceeded) -> [0]rt;

fr1[1] -> ICMPError(172.16.101.11, timeexceeded) -> [0]rt;
fr2[1] -> ICMPError(172.16.102.11, timeexceeded) -> [0]rt;
fr3[1] -> ICMPError(172.16.103.11, timeexceeded) -> [0]rt;

arpq08 -> out08;
arpq09 -> out09;
arpq10 -> out10;

