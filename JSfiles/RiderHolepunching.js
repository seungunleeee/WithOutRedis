var net = require("net");

var dgram = require("dgram");
var UDPsocket = dgram.createSocket("udp4");

var retPort;
var retAddress;
UDPsocket.bind(3000);
UDPsocket.on("listening", function () {
  console.log("listening event");
});

var msg = "HELLO!\0";
UDPsocket.send(msg, 0, msg.length, 3001, "15.165.22.113", function (err) {
  if (err) {
    console.log("UDP message send error", err);
    return;
  }
});

UDPsocket.on("message", function (msg, rinfo) {
  console.log("!! 응답 : " + msg, " info : " + rinfo);
  // var result = JSON.parse(msg);
  // retAddress = result.address;
  // retPort = result.port;
  // socket.write(
  //   "Rider,Enrollment,Daegu_Susung_Bumadong-" + retAddress + "-" + retPort
  // );
  var msg = "HELLO!\0";

  UDPsocket.send(msg, 0, msg.length, 3001, "49.143.19.137", function (err) {
    if (err) {
      console.log("UDP message send error", err);
      return;
    }
  });

  return;
});

// 서버 5000번 포트로 접속
// var socket = net.connect({ port: 8080, host: "127.0.0.1" });

// socket.on("connect", function () {
//   console.log("connected to server!");

// 1000ms의 간격으로 banana hong을 서버로 요청
// setInterval(function () {
//   socket.write("Rider Daegu/susung/bumaudong");
// }, 3000);
// });

// 서버로부터 받은 데이터를 화면에 출력
// socket.on("data", function (chunk) {
//   console.log("recv:" + chunk);
// });
// // 접속이 종료됬을때 메시지 출력
// socket.on("end", function () {
//   console.log("disconnected.");
// });
// // 에러가 발생할때 에러메시지 화면에 출력
// socket.on("error", function (err) {
//   console.log(err);
// });
// // connection에서 timeout이 발생하면 메시지 출력
// socket.on("timeout", function () {
//   console.log("connection timeout.");
// });
