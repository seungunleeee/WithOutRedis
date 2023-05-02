var dgram = require("dgram");
var socket = dgram.createSocket("udp4");

socket.bind(3001);

socket.on("listening", function () {
  console.log("listening event");
});

socket.on("message", function (msg, rinfo) {
  console.log("메세지 도착", rinfo.address, msg.toString());
  var result = JSON.stringify(rinfo);
  console.log("Stringfy 결과 : " + result);
  //   var msg = new Buffer(
  //     "{IP : " + rinfo.address + ", PORT : " + rinfo.port + "} \0"
  //   );
  socket.send(
    result,
    0,
    result.length,
    rinfo.port,
    rinfo.address,
    function (err) {
      if (err) {
        console.log("UDP message send error", err);
        return;
      }
      console.log("메세지 전송 성공");
    }
  );
});

socket.on("close", function () {
  console.log("close event");
});
