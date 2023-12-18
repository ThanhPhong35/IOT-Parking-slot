const char* loginPage PROGMEM = R"html(
<!DOCTYPE html>
<html>
<head>
  <title>LOGIN</title>
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
        margin-top: 50px;
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
        width: 295px;
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
        font-size: 20px;
        cursor: pointer;
        background: linear-gradient(to right, #ffd400,#ffa500);
    }

    #content .div1 input[type="checkbox"]{
        margin-right: 5px;
        width: 20px;
    }
    #content .div1 .showpass{
        margin-top: 10px;
        font-size: 18px;
    }

    #content .select{
        display: flex;
        justify-content: space-between;
    }
  </style>
  <meta name="viewport" content="width=device-width,user-scalable=0"  charset="utf-8">
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
      <div class="title">
        <p>Đăng nhập</p>
      </div>
      <div class="div1" >
          <p class="label">Tên đăng nhập:</p>
          <input id="user_login" class="input" type="text"><br>
          <p class="label">Mật khẩu:</p>
          <input id="pass_login" class="input" type="password">
          <div class="showpass">
            <input onclick="showpass('pass_login')" type="checkbox"><span>Hiện mật khẩu</span>
          </div>
      </div>
      
      <div id="btlogin" class="div1 select">
        <input onclick="checkLogin('1')" type="button" value="Đăng nhập">
        <input onclick="checkLogin('2')" type="button" value="Đổi mật khẩu">
      </div>

      <div id="btupdate" class="div1 select">
        <input  onclick="saveLogin()" type="button" value="Lưu">
        <input  onclick="window.location.href='/'" type="button" value="Thoát">
      </div>
    </div>    
  </div>

  <script type="text/javascript">
    window.onload=function(){
      document.getElementById("btlogin").style.display="block";
      document.getElementById("btupdate").style.display="none";
    }

    var xhttp = new XMLHttpRequest();
    function checkLogin(m){
      user_login = document.getElementById("user_login").value;
      pass_login = document.getElementById("pass_login").value;
      xhttp.onreadystatechange = function(){
        if(xhttp.readyState==4&&xhttp.status==200){
          var loginState = xhttp.responseText;
          if(loginState=="OK"){
            sessionStorage.setItem("LOGIN_STATE","1");
            if(m=='1'){
              window.location.assign('/');
            }else{
              document.getElementById("btlogin").style.display="none";
              document.getElementById("btupdate").style.display="block";
            }
          }else{
            sessionStorage.setItem("LOGIN_STATE","0");
            alert(loginState);
          }
        }
      }
      xhttp.open("GET","/checkLogin?user_login="+user_login + "&pass_login=" + pass_login,true);
          xhttp.send();
    }

    function saveLogin(){
      user_login = document.getElementById("user_login").value;
      pass_login = document.getElementById("pass_login").value;
      xhttp.onreadystatechange = function(){
        if(xhttp.readyState==4&&xhttp.status==200){
          alert(xhttp.responseText);
        }
      }
      xhttp.open("GET","/saveLogin?user_login="+user_login + "&pass_login=" + pass_login,true);
          xhttp.send();
    }

    function showpass(m){
          var temp = document.getElementById(m);
          if(temp.type=="password"){
            temp.type = "text";
          }else{
            temp.type="password";
         }
      }
  </script>
</body>
</html>
)html";
