var net = require("net");

var dgram = require("dgram");
var UDPsocket = dgram.createSocket("udp4");

var retPort;
var retAddress;
UDPsocket.bind(3004);
UDPsocket.on("listening", function () {
  console.log("listening event");
});

UDPsocket.on("message", function (msg, rinfo) {
  console.log("!! 라이더로부터의 응답 : " + msg, " info : " + rinfo);
});

var socket = new net.Socket();

socket = net.connect({ port: 8080, host: "15.165.129.230" });
var msg = "Administrator,Enrollment,Daegu_Susung_Bumadong-" + 3004 + "\0";
socket.on("connect", function () {
  socket.write(msg);
});

// 서버로부터 받은 데이터를 화면에 출력
socket.on("data", function (chunk) {
  console.log("recv: 관제자가 받을 라이더의 UDP 정보" + chunk);

  var INFO = chunk.toString().split("|");
  INFO.forEach((element) => {
    console.log("파싱후의 값 : " + element);
  });
  var toRider = "HELLO";
  UDPsocket.send(
    toRider,
    0,
    toRider.length,
    parseInt(INFO[1]),
    INFO[0],
    function (err) {
      if (err) {
        console.log("UDP message send error", err);
        return;
      }
    }
  );
});
// 접속이 종료됬을때 메시지 출력
socket.on("end", function () {
  console.log("disconnected.");
});
// 에러가 발생할때 에러메시지 화면에 출력
socket.on("error", function (err) {
  console.log(err);
});
// connection에서 timeout이 발생하면 메시지 출력
socket.on("timeout", function () {
  console.log("connection timeout.");
});

//비스포크 냉장고
// socket.end();
// var IPlist = [];
// var Portlist = [];
// const str = String(chunk);
// var temp = [];

// temp = str.split("-");

// for (var i = 0; i < temp.length; i++) {
//   console.log("결과 : " + temp[i]);
//   var UDPINFO = [];
//   UDPINFO = temp[i].split("|");
//   IPlist.push(UDPINFO[0]);
//   Portlist.push(UDPINFO[1]);
//   console.log("결과 값: " + IPlist[i] + " : " + Portlist[i]);
// }

// for (var i = 0; i < IPlist.length - 1; i++) {
//   var msg = Buffer("Hello "); // 공유기 IP테이블에 등록용입니다.
//   console.log("타입이뭐니???" + typeof Number(Portlist[i]));
//   console.log("port number :" + Number(Portlist[i]));
//   UDPsocket.send(
//     msg,
//     0,
//     msg.length,
//     Number(Portlist[i]),
//     IPlist[i],
//     function (err) {
//       if (err) {
//         console.log("UDP message send error", err);
//         return;
//       }
//       console.log("UDP 정보 공유기에 등록!!");
//     }
//   );
// }
