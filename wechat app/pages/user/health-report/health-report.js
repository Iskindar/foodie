var util = require("../../../utils/util.js");
var wxCharts = require('../../../utils/wxcharts.js');
Page({
  data: {},
  onLoad: function (options) {
    var totalScore = 90;
    var percent = totalScore * 0.02 - 0.5;
    util.makeArc(percent, '#d2d2d2', '#ffffff');
  },
  onReady: function () {
    // 页面渲染完成
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