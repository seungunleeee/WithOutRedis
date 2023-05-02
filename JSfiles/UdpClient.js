var dgram = require("dgram");
var UDPsocket = dgram.createSocket("udp4");

UDPsocket.bind(3005);
UDPsocket.on("listening", function () {
  console.log("listening event");
});

var msg = "hello!";
UDPsocket.send(msg, 0, msg.length, 13830, "117.111.22.247", function (err) {
  console.log(err);
  if (err) {
    console.log("UDP message send error", err);
    return;
  }
  UDPsocket.log("메세지 전송 성공");
});

UDPsocket.on("message", function (msg, rinfo) {
  console.log("응답 : " + msg, " info : " + rinfo);
});
