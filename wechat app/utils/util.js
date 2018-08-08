function formatTime(date) {
  var year = date.getFullYear()
  var month = date.getMonth() + 1
  var day = date.getDate()
  var hour = date.getHours()
  var minute = date.getMinutes()
  var second = date.getSeconds()


  return [year, month, day].map(formatNumber).join('/') + ' ' + [hour, minute, second].map(formatNumber).join(':')
}

function formatNumber(n) {
  n = n.toString()
  return n[1] ? n : '0' + n
}
//提示
function makeToast() {
  wx.showToast({
    title: '加载中',
    icon: 'loading',
    duration: 2000
  })
}
//圆形进度条
function makeArc(percent,downcolor,upcolor) {
  var cxt_arc = wx.createCanvasContext('canvasArc');//创建并返回绘图上下文context对象。 
  cxt_arc.setLineWidth(6);
  cxt_arc.setStrokeStyle(downcolor);
  cxt_arc.setLineCap('round')
  cxt_arc.beginPath();//开始一个新的路径 
  cxt_arc.arc(106, 56, 50, 0, 2 * Math.PI, false);//设置一个原点(106,106)，半径为100的圆的路径到当前路径 
  cxt_arc.stroke();//对当前路径进行描边 

  cxt_arc.setLineWidth(6);
  cxt_arc.setStrokeStyle(upcolor);
  cxt_arc.setLineCap('round')
  cxt_arc.beginPath();//开始一个新的路径 
  cxt_arc.arc(106, 56, 50, -Math.PI * 1 / 2, Math.PI * percent, false);
  cxt_arc.stroke();//对当前路径进行描边 

  cxt_arc.draw();

}
//画饼图
function makePie(weight1, weight2, weight3) {
  // 创建上下文
  var context = wx.createContext();
  // 画饼图
  //  数据源
  var array = [weight1, weight2, weight3];
  var colors = ["#1195db", "#112079", "#87a7d6"];
  var total = 0;
  //  计算问题
  for (var index = 0; index < array.length; index++) {
    total += array[index];
  }
  //  定义圆心坐标
  var point = { x: 160, y: 160 };
  //  定义半径大小
  var radius = 100;

  /*  循环遍历所有的pie */
  for (var i = 0; i < array.length; i++) {
    context.beginPath();
    //    起点弧度
    var start = 0;
    if (i > 0) {
      //      计算开始弧度是前几项的总和，即从之前的基础的上继续作画
      for (var j = 0; j < i; j++) {
        start += array[j] / total * 2 * Math.PI;
      }
    }
    console.log("i:" + i);
    console.log("start:" + start);
    //   1.先做第一个pie
    //    2.画一条弧，并填充成三角饼pie，前2个参数确定圆心，第3参数为半径，第4参数起始旋转弧度数，第5参数本次扫过的弧度数，第6个参数为时针方向-false为顺时针
    context.arc(point.x, point.y, radius, start, array[i] / total * 2 * Math.PI, false);
    //   3.连线回圆心
    context.lineTo(point.x, point.y);
    //   4.填充样式
    context.setFillStyle(colors[i]);
    //   5.填充动作
    context.fill();
    context.closePath();
  }

  wx.drawCanvas({
    canvasId: 'canvas2',
    actions: context.getActions()
  });
}
//保留两位小数
function returnFloat(value) {
  var value = Math.round(parseFloat(value) * 100) / 100;
  var xsd = value.toString().split(".");
  if (xsd.length == 1) {
    value = value.toString() + ".00";
    return value;
  }
  if (xsd.length > 1) {
    if (xsd[1].length < 2) {
      value = value.toString() + "0";
    }
    return value;
  }
}
function calUx(intake ,RNI,LTI) {
  var Ux;
  if (intake == 0) {
    Ux = 0;
  } else if (intake > 0 && intake < RNI) {
    Ux = 1 / (1 + Math.pow(LTI / intake, 10));
  } else if (intake > RNI && intake < 1.2 * RNI) {
    Ux = 1;
  } else {
    Ux = 1 / (1 + ((subject.intake - 1.2 * RNI) / (2.5 * RNI)));
  }
  return Ux;
}
function getNowFormatDate() {
  var date = new Date();
  var seperator1 = "-";
  var seperator2 = ":";
  var month = date.getMonth() + 1;
  var strDate = date.getDate();
  if (month >= 1 && month <= 9) {
    month = "0" + month;
  }
  if (strDate >= 0 && strDate <= 9) {
    strDate = "0" + strDate;
  }
  var currentdate = date.getFullYear() + seperator1 + month + seperator1 + strDate
    + " " + date.getHours() + seperator2 + date.getMinutes()
    + seperator2 + date.getSeconds();
  return currentdate;
} 
module.exports = {
  formatTime: formatTime,
  makeToast: makeToast,
  makePie: makePie,
  makeArc: makeArc,
  returnFloat: returnFloat,
  calUx:calUx,
  getNowFormatDate: getNowFormatDate
}
