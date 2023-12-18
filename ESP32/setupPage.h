const char setupPage[] PROGMEM = R"=====( 
<!DOCTYPE html>
<html>
<head>
  <title>SETUP WIFI</title>
  <style type="text/css">
    html{
        font-size: 62.5%;
    }

    body{
       font-size: 1.6em;
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

    #content .title p{
        font-size: 3em;
        text-align: center;
        font-weight: 500;
    }

    #content .div1 .label{
        margin-top: 20px;
        margin-bottom: 8px;
        font-size: 20px;
    }

    #content .div1 input[type="text"],
    #content .div1 input[type="password"]{
        width: 400px;
        padding: 5px 10px;
        font-size: 15px;
        border-radius: 5px;
        border: 2px solid #ffa500;
    }
    
    #content .div1 input[type="button"]{
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
    #content .div1 .showpass{
        margin-top: 5px;
        margin-bottom: 20px;
        font-size: 18px;
    }

    #ssidList, 
    #ssidList option
    {
      padding: 5px;
      border-radius: 5px;
      border: 2px solid #ffa500;
    }

    #content .select{
        display: flex;
        justify-content: space-between;
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
      <div class="div1">
        <h3>CHẾ ĐỘ PHÁT WIFI</h3>
          <p class="label">Tên wifi phát:</p>
          <input id="ssid_ap" readonly="true" class="input" type="text"><br>
          <p class="label">Mật khẩu:</p>
          <input id="pass_ap" readonly="true" class="input" type="password">
          <div class="showpass">
            <input onclick="showpass('pass_ap')" type="checkbox"><span>Hiện mật khẩu</span>
          </div>
      </div>
      <div class="div1">
        <h3>CHẾ ĐỘ KẾT NỐI WIFI</h3>
          <p class="label">Tên wifi kết nối:</p>
          <input id="ssid_sta" readonly="true" class="input" type="text"><br><br>
          <select id="ssidList" readonly="true" onchange="updatessid_sta()">
            <option >Chọn mạng wifi</option>
          </select>
          <p class="label">Mật khẩu:</p>
          <input id="pass_sta" readonly="true" class="input" type="password">
          <div class="showpass">
            <input onclick="showpass('pass_sta')" type="checkbox"><span>Hiện mật khẩu</span>
          </div>
      </div>
      <div class="div1 select">
        <input id="edit" type="button" onclick="getWifiList()" value="Sửa">
        <input id="save" type="button" onclick="saveConfig()" value="Lưu">
        <input type="button" onclick="window.location.href='/home'" value="Thoát">
        <input type="button" onclick="reStart()" value="Restart">
      </div>
  </div>
  <script type="text/javascript">
    window.onload = function(){
      if(sessionStorage.getItem("LOGIN_STATE") != "1"){
        window.location.assign('/login');
      }else{
        getConfig();
      }
    }
    var xhttp = new XMLHttpRequest();
    var ssid_ap, pass_ap, user_login, pass_login;
    function getConfig(){
      xhttp.onreadystatechange = function(){
        if(xhttp.readyState==4 && xhttp.status==200){
          //alert(xhttp.responseText);
          var obj = JSON.parse(xhttp.responseText);
          ssid_ap = obj.ssid_ap;
          pass_ap = obj.pass_ap;
          user_login = obj.user_login;
          pass_login = obj.pass_login;
          ssid_sta = obj.ssid_sta;
          pass_sta = obj.pass_sta;
          document.getElementById("ssid_ap").value=ssid_ap;
          document.getElementById("pass_ap").value=pass_ap;
          if(ssid_sta!=null){
            document.getElementById("ssid_sta").value = ssid_sta;
            document.getElementById("pass_sta").value = pass_sta;
          }
        }
      }
      xhttp.open("GET","/getConfig",true);
      xhttp.send();
    }

    function getWifiList(){
      document.getElementById("ssid_ap").readOnly=false;
      document.getElementById("pass_ap").readOnly=false;
      document.getElementById("ssid_sta").readOnly=false;
      document.getElementById("pass_sta").readOnly=false;
      document.getElementById("save").style.display="block";
      document.getElementById("edit").style.display="none";
      xhttp.onreadystatechange = function(){
        if(xhttp.readyState==4 && xhttp.status==200){
          //alert(xhttp.responseText);
          var obj = JSON.parse(xhttp.responseText);
          ssidList = obj.ssidList;
          var select = document.getElementById("ssidList");
          for(var i=0; i<ssidList.length;++i){
            select[select.length] = new Option(ssidList[i],ssidList[i]);
          }
        }
      }
      xhttp.open("GET","/getWifiList",true);
      xhttp.send();
    }
    function updatessid_sta(){
      document.getElementById("ssid_sta").value = document.getElementById("ssidList").value;
    }
    function saveConfig(){
        const json = {
          ssid_ap: document.getElementById("ssid_ap").value,
          pass_ap: document.getElementById("pass_ap").value,
          user_login: user_login,
          pass_login: pass_login,
          ssid_sta: document.getElementById("ssid_sta").value,
          pass_sta: document.getElementById("pass_sta").value
        };
        const jsonString = JSON.stringify(json);
        xhttp.onreadystatechange = function(){
          if(xhttp.readyState==4 && xhttp.status==200){
            alert(xhttp.responseText);
            document.getElementById("ssid_ap").readOnly=true;
            document.getElementById("pass_ap").readOnly=true;
            document.getElementById("ssid_sta").readOnly=true;
            document.getElementById("pass_sta").readOnly=true;
            document.getElementById("save").style.display="none";
            document.getElementById("edit").style.display="block";
          }
        }
        xhttp.open("POST","/saveConfig",true);
        xhttp.setRequestHeader("Content-type","application/json");
        xhttp.send(jsonString);
    }
    function reStart(){
      if(confirm("Bạn có muốn khởi động lại thiết bị hay không?")){
        alert("Hệ thống đang khởi động lại vui lòng đợi trong giây lát!")
        xhttp.open("GET","/reStart",true);
        xhttp.send();
      }
    }
    function showpass(m){
      var temp = document.getElementById(m);
      if(temp.type=="password"){
        temp.type = "text";
      }else{
        temp.type="password";
      }
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
