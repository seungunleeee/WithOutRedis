var net = require("net");

// 서버 5000번 포트로 접속
var socket = net.connect({ port: 8080, host: "15.165.129.230" });
socket.on("connect", function () {
  console.log("connected to server!");

  // 1000ms의 간격으로 banana hong을 서버로 요청
  setInterval(function () {
    socket.write(
      "Rider,Enrollment,Daegu_Susung_Bumadong-" +
        "49.135.222.35" +
        "-" +
        8080 +
        "\0"
    );
  }, 3000);
});

// 서버로부터 받은 데이터를 화면에 출력
socket.on("data", function (chunk) {
  console.log("recv:" + chunk);
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
