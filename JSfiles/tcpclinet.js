const net = require("net");
const socket = net.connect({
  port: 8080,
  host: "15.165.129.230",
});
let today = new Date();
var dgram = require("dgram");
const { time, timeStamp } = require("console");
var UDPsocket = dgram.createSocket("udp4");

var msg = new Buffer("집에서 보내서 왔습니다. 날짜 : " + today);

let servdata = null;
// setting encoding
socket.setEncoding("utf8");

socket.on("connect", function () {
  console.log("on connect");

  // send message to server
  setInterval(() => {
    socket.write("msg from client , HEllo!");
  }, 3000);

  setTimeout(() => {
    // socket.destroy();
  }, 2000);
});

socket.on("data", function (data) {
  console.log(data);
  servdata = JSON.parse(data);
  console.log(servdata);
  console.log("보낼내용 : " + msg);
  UDPsocket.send(
    msg,
    0,
    msg.length,
    servdata["PORT"],
    servdata["IP"],
    function (err) {
      console.log(err);
      if (err) {
        console.log("UDP message send error", err);
        return;
      }
      console.log("메세지 전송 성공");
      UDPsocket.close();
    }
  );
  socket.destroy();
});

socket.on("close", function () {
  console.log("close");
});

socket.on("error", function (err) {
  console.log("on error: ", err.code);
});
