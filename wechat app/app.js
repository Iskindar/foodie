var Bmob = require('utils/bmob.js');
Bmob.initialize("b844aa80cbcb1922ea5e20520479ceac", "bbfce6df9fabee330290845135cb5c0d");

App({
  onLaunch: function () {
    var user = new Bmob.User();//开始注册用户

    var logs = wx.getStorageSync('logs') || []
    logs.unshift(Date.now())
    wx.setStorageSync('logs', logs)
  },
  getUserInfo: function (cb) {
    var that = this
    if (this.globalData.userInfo) {
      typeof cb == "function" && cb(this.globalData.userInfo)
    } else {
      //调用登录接口
      wx.login({
        success: function () {
          wx.getUserInfo({
            withCredentials:true,
            success: function (res) {
              that.globalData.userInfo = res.userInfo
              typeof cb == "function" && cb(that.globalData.userInfo)
            }
          })
        }
      })
    }
  },
  globalData: {
    userInfo: null
  }
})
