var app = getApp();
Page({
  data: {
    userInfo: {}
  },
  onLoad: function (options) {
    var that = this;
    app.getUserInfo(function (userInfo) {
      //更新数据
      console.log(userInfo);
      that.setData({
        userInfo: userInfo
      })
    })
  },
  onBindingTap: function () {
    wx.showModal({
      title: '提示',
      content: '绑定成功',
      showCancel:false,
      success: function (res) {
        if (res.confirm) {
          console.log('用户点击了确定')
         wx.navigateBack({
           delta:1
         })
        }
      }
    })
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