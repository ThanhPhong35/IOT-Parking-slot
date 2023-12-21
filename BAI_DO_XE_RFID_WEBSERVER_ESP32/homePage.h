const char homePage[] PROGMEM = R"rawliteral( 
<!DOCTYPE html>
<html>
<head>
  <title>HOME</title>
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style type="text/css">


    body{
       font-family: prevattscriptssk;
       background-color: #f3f3f3;
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
        width: 100vw;
        margin-top: 30px;
        display: flex;
        justify-content: center;
        align-items: center;
    }
    
    #content{
        border: 2px solid #ffa500;
        background-color: white;
        padding: 20px;
        border-radius: 5px;
    }

    #content .chart{
      width: max-content;
      margin: 20px auto;
      padding: 20px;
      border-radius: 8px;
      border: 2px solid #ffa500;
      cursor: pointer;
      font-size: 20px;
    }

    #content .chart:hover{
      background-color: aquamarine;
    }

    #content .rfid{
      padding: 20px;
      background-color: #059e8a;
      border-radius: 5px;
      color: white;
    }
    #content .rfid h3{
      text-align: center;
      padding: 5px;
    }

    #content .rfid table td{
      padding: 5px;
    }

    #content .rfid .label{
        margin-right: 10px;
        font-size: 18px;
        color: white;
    }

    #content .rfid input[type="text"],
    #content .rfid input[type="password"]{
        width: 300px;
        padding: 3px 10px;
        font-size: 15px;
        border-radius: 5px;
        border: 1.5px solid #ffa500;

    }

    #content .menu{
      display: flex;
      justify-content: space-evenly;
    }
    
    #content .menu input[type="button"]{
        margin: 10px 0;
        padding: 7px 20px;
        border: none;
        border-radius: 5px;
        font-size: 18px;
        cursor: pointer;
        background: linear-gradient(to right, #ffd400,#ffa500);
    }

    #content .div1 input[type="checkbox"]{
        margin-right: 5px;
        width: 20px;
    }

    #content .rfid,
    #content .div2
    {
      margin: auto;
      width: max-content;
    }

    #content .div2 .row1,
    #content .div2 .row2
    {
      display: grid;
      grid-template-columns: repeat(3,1fr);
      gap: 30px;
    }

    #content .div2 .row1 input{
      margin-top: 30px;
      margin-bottom: 10px;
      width: 60px;
      height: 60px;
      text-align: center;
      font-size: 25px;
      border-radius: 50%;
      border-color: red;
      margin-left: 80px;
    }

    #content .div2 .row2 input{
      margin: 3px 0;
      width: 230px;
      text-align: center;
      padding: 3px;
      outline: none;
      border: none;
    }

  </style>
  <meta name="viewport" content="width=device-width,user-scalable=0" charset="utf-8">
</head>
<body>
  <div id="header">
    <p class="p1"> Học Viện Công Nghệ Bưu Chính Viễn Thông</p>
    <p class="p2"> Bài tập kết thúc học phẩn IOT </p>
    <p class="p3"> ĐỀ TÀI: IOT CAR PARKING </p>
    <p class="p4"> SVTH: Nguyễn Thị Quyên, Trần Đăng Trọng </p>
</div>
  <div id="main">
    <div id="content">
        <div class="menu">
            <input type="button" onclick="window.location.href='/setup'" value="Thiết lập wifi">
            <input type="button" onclick="window.location.href='/data'" value="Thiết lập thẻ RFID">
            <input type="button" onclick="exit()" value="Thoát">
          </div>
          <div class="chart" onclick="window.location.href='/chart'">
            <p>
              <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
              <span class="dht-labels">Nhiệt độ:</span> 
              <span id="temperature"></span>
              <sup class="units">&deg;C</sup>
            </p>
            <p>
              <i class="fas fa-tint" style="color:#00add6;"></i> 
              <span class="dht-labels">Độ ẩm:</span>
              <span id="humidity"></span>
              <sup class="units">&percnt;</sup>
            </p>
          </div>
        <div class="rfid">
          <div>
            <h3>THẺ RFID</h3>
            <table>
              <tr>
                <td>
                  <span class="label" >Mã thẻ:</span>
                </td>
                <td>
                  <input id="idcard" type="text" >
                </td>
              </tr>
              <tr>
                <td>
                  <span class="label">Tên thẻ:</span>
                </td>
                <td>
                  <input id="namecard" type="text">
                </td>
              </tr>
            </table>
          </div>
        </div>
        <div class="div2" >
          <div class="tablefull infor">
            <div class="row1">
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
                <input id="name2" class="textData" type="text" readonly="true" value=""><br>
                <input id="data2" class="textData" type="text" readonly="true" value=" ">
              </div>
              <div class="col3">
                <input id="name3" class="textData" type="text" readonly="true" value=" "><br>
                <input id="data3" class="textData" type="text" readonly="true" value=" ">
              </div>
            </div>
  
            <div class="row1">
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
      setInterval(function() {
        getDHT();
      }, 5000);
      
      function getDHT(){
        xhttp.onreadystatechange =function(){
          if(xhttp.readyState==4 && xhttp.status==200){
            var Temp = xhttp.responseText;
            var obj =JSON.parse(Temp);
            document.getElementById("temperature").innerHTML=obj.temp;
            document.getElementById("humidity").innerHTML=obj.hum;
          }
        }
        xhttp.open("GET", "/getDHT", true);
        xhttp.send();
      }

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
  )rawliteral";
