const char chartPage[] PROGMEM = R"=====( 
<!DOCTYPE HTML>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    body {
      min-width: 310px;
      max-width: 800px;
      margin: 0 auto;
    }
    h2 {
      font-family: Arial;
      font-size: 2.5rem;
      text-align: center;
    }
    canvas {
      border: 1px solid #ccc;
      margin: 30px;
    }
  </style>
</head>
<body>
  <h2>Thống kê thời tiết</h2>
  <canvas id="chart-temperature" width="800" height="200"></canvas>
  <canvas id="chart-humidity" width="800" height="200"></canvas>

  <script>
    const canvasT = document.getElementById('chart-temperature');
    const ctxT = canvasT.getContext('2d');

    const canvasH = document.getElementById('chart-humidity');
    const ctxH = canvasH.getContext('2d');

    const dataT = [];
    const dataH = [];

    setInterval(function () {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          var JSONtxt = xhttp.responseText;
            var obj =JSON.parse(JSONtxt);
          var x = (new Date()).getTime(),
              y = parseFloat(obj.temp),
              z = parseFloat(obj.hum);
          dataT.push({ x, y });
          dataH.push({ x, z });

          drawLineChart(ctxT, dataT, '#059e8a', 'Nhiệt độ (Celsius)');
          drawLineChart(ctxH, dataH, '#0000ff', 'Độ ẩm (%)');
        }
      };

      xhttp.open("GET", "/getDHT", true);
      xhttp.send();
    }, 2000);

    function drawLineChart(ctx, data, color, yAxisLabel) {
      const width = ctx.canvas.width;
      const height = ctx.canvas.height;

      ctx.clearRect(0, 0, width, height);

      const maxValue = Math.max(...data.map(point => point.y));
      const columnWidth = width / (data.length - 1);
      const columnHeightRatio = height / maxValue;

      ctx.beginPath();
      ctx.moveTo(0, height - data[0].y * columnHeightRatio);

      for (let i = 1; i < data.length; i++) {
        const x = i * columnWidth;
        const y = height - data[i].y * columnHeightRatio;
        ctx.lineTo(x, y);
      }

      ctx.strokeStyle = color;
      ctx.lineWidth = 2;
      ctx.stroke();

      // Vẽ trục x và y
      ctx.beginPath();
      ctx.moveTo(0, height);
      ctx.lineTo(width, height);
      ctx.moveTo(0, 0);
      ctx.lineTo(0, height);
      ctx.stroke();

      // Nhãn trục y
      ctx.fillStyle = '#000';
      ctx.fillText(yAxisLabel, 10, 10);
    }
  </script>
</body>
</html>
  )=====";
