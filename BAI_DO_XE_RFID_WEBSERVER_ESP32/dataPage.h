const char dataPage[] PROGMEM = R"=====( 
<!DOCTYPE html>
<html>
<head>
  <title>SETUP RFID</title>
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
        margin-right: 10px;
        font-size: 20px;
    }

    #content .div1 input[type="text"],
    #content .div1 input[type="password"]{
        width: 300px;
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

    #content .select{
        display: flex;
        justify-content: space-evenly;
        margin-top: 10px;
        margin-bottom: 30px;
    }
    #dataStore{
      height: 200px;
      width: 380px;
      overflow-y: scroll;
      border: solid 1px #ffa500;
    }
    h3{
        margin-bottom: 10px;
        text-align: center;
    }
    table{
      width: 380px;
      height: 50px;
      float: left;
    }
    table, td 
    {
        border: solid 1px #DDD;
        border-collapse: collapse;
        padding: 2px 3px;
        text-align: left;
        font:14px Verdana;
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
        <h3>THÔNG TIN THẺ</h3>
          <p><span class="label">Mã thẻ:</span>
              <input id="idcard" type="text"></p><br><br>
          <p><span class="label">Tên thẻ:</span>
              <input id="namecard" type="text"></p>
      </div>
      <div class="div1 select">
        <input type="button" onclick="addcard()" value="Thêm">
        <input type="button" onclick="deletecard()" value="Xóa">
        <input type="button" onclick="exit()" value="Thoát">
      </div>
      <div class="div1">
        <h3>DANH SÁCH THẺ RFID</h3>
        <div id="dataStore">
        </div>
      </div>
  </div>
  <script type="text/javascript">
    window.onload = function(){
       if(sessionStorage.getItem("LOGIN_STATE") != "1"){
         window.location.assign('/login');
       }else{
         getDataStore();
         init();
       }
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
       }
    }

    var xhttp = new XMLHttpRequest();
    function getDataStore(){
      xhttp.onreadystatechange = function(){
        if(xhttp.readyState==4 && xhttp.status==200){
          console.log("Data response:" ,xhttp.responseText);
          obj = JSON.parse(xhttp.responseText);
          window.data=obj;
          num=1;
          var table = "<table id='tableId'>";
          table += `<tr><td>STT</td><td>MÃ THẺ</td><td>TÊN THẺ</td></tr>`;
          for (let key in obj) {
            table += `<tr><td>${num}</td><td>${key}</td><td>${obj[key]}</td></tr>`;
            num++;
          }
          table += "</table>";
          document.getElementById("dataStore").innerHTML = table;

          var rows = document.getElementById("tableId").rows;
          for (i = 0; i < rows.length; i++) {
              rows[i].onclick = function(){ return function(){
                     var id = this.cells[1].innerHTML;
                     var name= this.cells[2].innerHTML;
                     document.getElementById("idcard").value=id;
                     document.getElementById("namecard").value=name;
              };}(rows[i]);
          }
        }
      }
      xhttp.open("GET","/getDataStore",true);
      xhttp.send();
    }
    function deletecard(){
      var id = document.getElementById("idcard").value;
      if(Empty(id,"Vui lòng nhập mã thẻ!")){
        if(confirm("Bạn muốn xóa thẻ này?")){
          xhttp.onreadystatechange = function(){
              if(xhttp.readyState==4 && xhttp.status==200){
              //alert(xhttp.responseText);
                if(xhttp.responseText=="Đã xóa dữ liệu!"){
                  document.getElementById("idcard").value="";
                  document.getElementById("namecard").value="";
                  alert("Đã xóa thẻ!");
                  getDataStore();
                }
              }
            }
            xhttp.open("GET","/deleteDataStore?idcard="+id,true);
            xhttp.send();
        }
      }
    }
    
    function addcard(){
        var id = document.getElementById("idcard").value;
        var name = document.getElementById("namecard").value;
        if(Empty(id,"Vui lòng nhập mã thẻ!")&&Empty(name,"Vui lòng nhập vị trí!")){
          xhttp.onreadystatechange = function(){
            if(xhttp.readyState==4 && xhttp.status==200){
              //alert(xhttp.responseText);
              if(xhttp.responseText=="Đã thêm dữ liệu mới!"){
                document.getElementById("idcard").value="";
                document.getElementById("namecard").value="";
                getDataStore();
              }
            }
          }
          xhttp.open("GET","/addDataStore?idcard="+id+"&&namecard="+name,true);
          xhttp.send();
      }

    }

    function exit(){
      //sessionStorage.setItem("LOGIN_STATE","0");
      window.location.assign('/home');
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
