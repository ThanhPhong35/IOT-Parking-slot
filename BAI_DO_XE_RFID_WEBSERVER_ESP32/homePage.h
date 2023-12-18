const char homePage[] PROGMEM = R"=====( 
<!DOCTYPE html>
<html>
<head>
  <title>HOME</title>
  <style type="text/css">

    body{
       font-family: prevattscriptssk;
       text-decoration: none;
       list-style-type: none;
    }

    *{
        margin: 0;
        padding: 0;
        box-sizing: border-box;
    }
    
    #header{
        margin: auto;
        height: max-content;
        text-align: center;
        padding: 20px;
        background-color:#f9b639;
    }

    #header p
    {
        font-size: 30px;
        padding: 5px;
    }

    #header .p3
    {
        font-size: 25px;
        color: green;
    }

    #header .p4
    {
        font-size: 25px;
    }

    #main{
      margin: 5px auto;
      width: 728px;
    }
    #content{
      width: 728px;
      float: left;
      border-radius: 5px;
      color: blue;
      margin-left: 80px;
    }
    h3{
      margin-top: 0px;
      margin-bottom: 0px;
    }
    p{
      margin-top: 10px;
      margin-bottom: 10px;
    }
    .div1{
      margin-left: 20px;
      float: left;
      margin-bottom: 10px;
    }

    .div2 div{
      margin-left: 20px;
    }

    .rfid{
      margin-left:0;
      background-color: aquamarine;
      padding: 20px;
      border-radius: 5px;
    }
    .label{
      float: left;
      padding-right: 10px;
      font-size: 18px;
    }

    #content .div1 table input[type="text"],
    #content .div1 table [type="password"]{
        width: 300px;
        padding: 5px 10px;
        font-size: 15px;
        border-radius: 5px;
        border: 2px solid #ffa500;
    }
    input[type="button"]{
      width: 120px;
      height: 50px;
      float: left;
      margin: 10px 5px;
      outline: none;
      text-align: center; 
      white-space: normal;
      font-weight: bold;
      color: blue;
      background: white;
      border-radius: 5px;
      border-color: blue;
    }
    input[type="text"]{
      width: 270px;
      float: left;
      margin: 5px 0px;
    }
    .tablefull{
      width: 600px;
      height: 280px;
      float: left;
      margin: 3px 4px;
      outline: none;
      text-align: center; 
      white-space: normal;
      font-weight: bold;
      border: solid 1px #ffa500;
      border-radius: 8px;
    }
    .row1{
      width: 560px;
      float: left;
      /*border: solid 1px red;*/
    }
    .row2{
      width: 560px;
      float: left;
      /*border: solid 1px red;*/
    }
    .col1{
      width: 150px;
      float: left;
    }
    .col2{
      width: 150px;
      float: left;
    }
    .col3{
      width: 150px;
      float: left;
    }
    input[type="text"].textData{
      width: 160px;
      float: left;
      text-align: center;
      margin: 0px 0px;
      outline: none;
      border: none;
    }
    input[type="text"].led{
      width: 60px;
      height: 60px;
      font-size: 20px;
      border: solid red 2px;
      border-radius: 50%;
      outline: none;
      text-align: center; 
      float: left;
      margin-left: 50px;
    }
  </style>
  <meta name="viewport" content="width=device-width,user-scalable=0" charset="utf-8">
</head>
<body>
  <div id="main">
    <div id="header">
        <p class="p1"> Học Viện Công Nghệ Bưu Chính Viễn Thông</p>
        <p class="p2"> Bài tập kết thúc học phẩn IOT </p>
        <p class="p3"> ĐỀ TÀI: IOT CAR PARKING </p>
        <p class="p4"> SVTH: Nguyễn Thị Quyên, Trần Đăng Trọng </p>
    </div>
    <div id="content">
        <div class="div1" style="margin-left: 100px;">
            <input style="background: green; color: white;border-color: orange;" type="button" onclick="window.location.href='/setup'" value="Thiết lập wifi">
            <input style="background: yellow; color: green;border-color: orange;" type="button" onclick="window.location.href='/data'" value="Thiết lập thẻ RFID">
            <input style="background: red; color: white;border-color: orange;" type="button" onclick="exit()" value="Thoát">
          </div>
        <div class="div1 rfid" style="margin-left: 80px; margin-top: 20px;">
          <div>
            <h3 style="text-align: center;">THẺ RFID</h3>
            <table>
              <tr>
                <td>
                  <span class="label" >Mã thẻ:</span>
                </td>
                <td>
                  <input id="idcard" type="text" style="padding: 3px 10px;">
                </td>
              </tr>
              <tr>
                <td>
                  <span class="label">Tên thẻ:</span>
                </td>
                <td>
                  <input id="namecard" type="text" style="padding: 3px 10px;">
                </td>
              </tr>
            </table>
          </div>
        </div>
        <div class="div2" style="margin-left: -40px; margin-top: 20px;">
          <div class="tablefull infor">
            <div class="row1" style="margin-top: 20px;">
              <div class="col1">
                <input id="led1" class="led" type="text" readonly="true" value="1">
              </div>
              <div class="col2">
                <input id="led2" class="led" type="text" readonly="true" value="2">
              </div>
              <div class="col3">
                <input id="led3" class="led" type="text" readonly="true" value="3">
              </div>
            </div>
            <div class="row2">
              <div class="col1">
                <input id="name1" class="textData" type="text" readonly="true" value=" "><br>
                <input id="data1" class="textData" type="text" readonly="true" value=" ">
              </div>
              <div class="col2">
                <input id="name2" class="textData" type="text" readonly="true" value=" "><br>
                <input id="data2" class="textData" type="text" readonly="true" value=" ">
              </div>
              <div class="col3">
                <input id="name3" class="textData" type="text" readonly="true" value=" "><br>
                <input id="data3" class="textData" type="text" readonly="true" value=" ">
              </div>
            </div>
  
            <div class="row1" style="margin-top: 40px;">
              <div class="col1">
                <input id="led4" class="led" type="text" readonly="true" value="4">
              </div>
              <div class="col2">
                <input id="led5" class="led" type="text" readonly="true" value="5">
              </div>
              <div class="col3">
                <input id="led6" class="led" type="text" readonly="true" value="6">
              </div>
            </div>
            <div class="row2">
              <div class="col1">
                <input id="name4" class="textData" type="text" readonly="true" value=" "><br>
                <input id="data4" class="textData" type="text" readonly="true" value=" ">
              </div>
              <div class="col2">
                <input id="name5" class="textData" type="text" readonly="true" value=" "><br>
                <input id="data5" class="textData" type="text" readonly="true" value=" ">
              </div>
              <div class="col3">
                <input id="name6" class="textData" type="text" readonly="true" value=" "><br>
                <input id="data6" class="textData" type="text" readonly="true" value=" ">
              </div>
            </div>
          </div> 
        </div>
        
    </div>
    <script type="text/javascript">
      window.onload = function(){
         if(sessionStorage.getItem("LOGIN_STATE") != "1"){
           window.location.assign('/login');
         }else{
           getDataParking();
           init();
         }
      }
      var xhttp = new XMLHttpRequest();
      function getDataParking(){
        xhttp.onreadystatechange = function(){
          if(xhttp.readyState==4 && xhttp.status==200){
            obj = JSON.parse(xhttp.responseText);
            document.getElementById("led1").style.background="white";
            document.getElementById("name1").value="Empty";
            document.getElementById("data1").value="";
            document.getElementById("led2").style.background="white";
            document.getElementById("name2").value="Empty";
            document.getElementById("data2").value="";
            document.getElementById("led3").style.background="white";
            document.getElementById("name3").value="Empty";
            document.getElementById("data3").value="";
            document.getElementById("led4").style.background="white";
            document.getElementById("name4").value="Empty";
            document.getElementById("data4").value="";
            document.getElementById("led5").style.background="white";
            document.getElementById("name5").value="Empty";
            document.getElementById("data5").value="";
            document.getElementById("led6").style.background="white";
            document.getElementById("name6").value="Empty";
            document.getElementById("data6").value="";
            for (let key in obj) {
              var indexNum = obj[key][1];
              var nameData = obj[key][0];
              var timeData =  obj[key][2];
              var dateData =  obj[key][3];
              console.log("Name:" ,nameData);
              console.log("Time:" ,timeData);
              switch(indexNum){
                case 0:
                  document.getElementById("led1").style.background="yellow";
                  document.getElementById("name1").value=nameData;
                  document.getElementById("data1").value=dateData+" "+timeData;
                  break;
                case 1:
                  document.getElementById("led2").style.background="yellow";
                  document.getElementById("name2").value=nameData;
                  document.getElementById("data2").value=dateData+" "+timeData;
                  break;
                case 2:
                  document.getElementById("led3").style.background="yellow";
                  document.getElementById("name3").value=nameData;
                  document.getElementById("data3").value=dateData+" "+timeData;
                  break;
                case 3:
                  document.getElementById("led4").style.background="yellow";
                  document.getElementById("name4").value=nameData;
                  document.getElementById("data4").value=dateData+" "+timeData;
                  break;
                case 4:
                  document.getElementById("led5").style.background="yellow";
                  document.getElementById("name5").value=nameData;
                  document.getElementById("data5").value=dateData+" "+timeData;
                  break;
                case 5:
                  document.getElementById("led6").style.background="yellow";
                  document.getElementById("name6").value=nameData;
                  document.getElementById("data6").value=dateData+" "+timeData;
                  break;
              }
  
            }
            console.log("Prepared data:" ,obj);
          }
        }
        xhttp.open("GET","/getDataParking",true);
        xhttp.send();
      }
  
      var Socket;      //Khai báo biến Socket
      var temp=0;
      function init(){
         //Khởi tạo websocket
         Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
         //Nhận broadcase từ server
         Socket.onmessage = function(event){   
           JSONobj = JSON.parse(event.data);   //Tách dữ liệu json
           //alert(event.data);
           idcard = JSONobj.idcard;
           namecard = JSONobj.namecard;
           document.getElementById("idcard").value = idcard;
           document.getElementById("namecard").value = namecard;
           getDataParking();
         }
      }
  
      function sendControl(ctr){
        Socket.send(ctr);
      }
  
  
      function exit(){
        sessionStorage.setItem("LOGIN_STATE","0");
        window.location.assign('/login');
      }
  
      function Empty(element,message){
        if(element.trim()==""){
          alert(message);
          element.focus();
          return false;
        }else{
          return true;
        }
      }
    </script>
  </body>
  </html>
  )=====";
