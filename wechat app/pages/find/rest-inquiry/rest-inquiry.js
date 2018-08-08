var foodData = require('../../../data/data_base.js');
Page({
  data: {
    array: ['一食堂', '二食堂', '三食堂', '四食堂', '五食堂', '六食堂'],

  },
  bindPickerChange: function (e) {
    
    this.setData({
      index: e.detail.value,
      foodData:foodData.foodList
    })
  },
  onLoad: function (options) {
    // 页面初始化 options为页面跳转所带来的参数
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