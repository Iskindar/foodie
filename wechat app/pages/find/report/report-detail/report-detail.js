var util = require("../../../../utils/util.js");
var wxCharts = require('../../../../utils/wxcharts.js');
Page({
  data: {},
  onLoad: function (options) {
    var totalScore = 90;
    var percent = totalScore * 0.02 - 0.5;
    util.makeArc(percent, '#d2d2d2', '#ffffff');
  },
  onReady: function () {
    var windowWidth = 320;
    try {
      var res = wx.getSystemInfoSync();
      windowWidth = res.windowWidth;
    } catch (e) {
      console.error('getSystemInfoSync failed!');
    }
    var categories = ['周一', '周二', '周三', '周四', '周五', '周六', '周日'];
    var data = [110,105,92,110,89,102,93];
    new wxCharts({
      canvasId: 'lineCanvas',
      type: 'line',
      categories: categories,
      animation: false,
      background: '#f5f5f5',
      series: [{
        name: '时间波动',
        data: data,
        format: function (val, name) {
          return val.toFixed(0) ;
        }
      }],
      xAxis: {
        disableGrid: true
      },
      yAxis: {
        title: '饮食时间波动幅度/%',
        format: function (val) {
          return val.toFixed(0)+'%';
        },
        min: 0
      },
      width: windowWidth,
      height: 200,
      dataLabel: false,
      dataPointShape: false
    });
    new wxCharts({
      canvasId: 'columnCanvas',
      type: 'column',
      animation: true,
      categories: ['周一', '周二', '周三', '周四', '周五', '周六','周日'],
      series: [{
        name: '卡路里',
        data: [1722, 1900, 2215, 2237, 1804, 1980,2356],
      }],
      yAxis: {
        format: function (val) {
          return val + '千卡';
        },
        min:0
      },
      xAxis: {
        disableGrid: false,
        type: 'calibration'
      },
      width: windowWidth,
      height: 250,
    });
    var windowWidth = 320;
    try {
      var res = wx.getSystemInfoSync();
      windowWidth = res.windowWidth;
    } catch (e) {
      console.error('getSystemInfoSync failed!');
    }

    new wxCharts({
      animation: true,
      canvasId: 'pieCanvas',
      type: 'pie',
  
      extra: {
        ringWidth: 25
      },
      series: [{
        name: '蛋白质',
        data: 35,
        stroke: false,
        color: "#7dc5eb"
      }, {
        name: '脂肪',
        data: 20,
        Stroke: true,
        color:"#87c38f"
      }, {
        name: '碳水化合物',
        data: 55,
        stroke: false,
        color:"#efb336"
      }],
      disablePieStroke: true,
      width: windowWidth,
      height: 200,
      dataLabel: true
    });
  },
  onShow: function () {
    // 页面显示
  },
  onHide: function () {
    // 页面隐藏
  },
  onUnload: function () {
    // 页面关闭
  }
})