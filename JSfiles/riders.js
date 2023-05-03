var net = require("net");

var dgram = require("dgram");
var UDPsocket = dgram.createSocket("udp4");
UDPsocket.bind(3000);
UDPsocket.on("listening", function () {
  console.log("listening event");
});
// var retPort;
// var retAddress;

// UDPsocket.on("listening", function () {
//   console.log("listening event");
// });

// var msg = "HELLO!";
// UDPsocket.send(msg, 0, msg.length, 3001, "15.165.22.113", function (err) {
//   if (err) {
//     console.log("UDP message send error", err);
//     return;
//   }
// });

// UDPsocket.on("message", function (msg, rinfo) {
//   console.log("!! 응답 : " + msg, " info : " + rinfo);
//   var result = JSON.parse(msg);
//   retAddress = result.address;
//   retPort = result.port.toString();
//   console.log("" + typeof retPort + " port :  " + retPort);
// var msg =
//   "Rider,Enrollment,Daegu_Susung_Bumadong-" +
//   retAddress +
//   "-" +
//   retPort +
//   "\0";

//   socket.write(msg);
// var msg = Buffer("HELLO!");
// UDPsocket.send(msg, 0, msg.length, 3002, "49.143.19.137", function (err) {
//   if (err) {
//     console.log("UDP message send error", err);
//     return;
//   }

//   socket.write("Rider Daegu/susung/bumaudong");
// });
//   return;
// });

// 서버 5000번 포트로 접속
var msg = "Rider,Enrollment,Daegu_Susung_Bumadong-" + 3000 + "\0";
// /15.165.129.230
var socket = net.connect({ port: 8080, host: "15.165.129.230" });

socket.on("connect", function () {
  console.log("connected to server!");
  socket.write(msg);
  //1000ms의 간격으로 banana hong을 서버로 요청
  // setInterval(function () {
  //   socket.write(msg);
  // }, 2000);
});

// 서버로부터 받은 데이터를 화면에 출력
socket.on("data", function (chunk) {
  var ans = chunk.toString();
  console.log("recv: 서버로 부터 받은 관제자의 UDP정보 " + ans);

  console.log("스트링으로 바꾼값: " + ans);
  // var data = [];
  // data = String(chunk).split("|");

  // var msg = "대구시 수성구 범어동 \0";
  // setInterval(() => {
  //   UDPsocket.send(msg, 0, msg.length, 36519, "117.111.4.77", function (err) {
  //     if (err) {
  //       console.log("UDP message send error", err);
  //       return;
  //     }
  //     console.log("보냈음!");
  //   });
  // }, 3000);

  console.log("비교 결과 : ", parseInt(ans, 10));
  if (parseInt(ans, 10) == parseInt("-1", 10)) {
    setTimeout(() => {
      console.log("전송전");
      socket.write(msg);
      console.log("전송완료");
    }, 3000);
  } else {
    var INFO = ans.split("|");
    INFO.forEach((element) => {
      console.log("파싱한 결과 : " + element);
    });
    console.log("UDP 전송시작");
    setInterval(() => {
      var location = "경기도_광명시_철산동";
      // UDPSEND시작
      console.log("UDP 전송");
      UDPsocket.send(
        location,
        0,
        location.length,
        parseInt(INFO[1]),
        INFO[0],
        function (err) {
          if (err) {
            console.log("UDP message send error", err);
            return;
          }
        }
      );
      console.log("UDP 전송 끝");
      //UDPSEND끝
    }, 3000);
  }
  console.log("여기까진옴");
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
