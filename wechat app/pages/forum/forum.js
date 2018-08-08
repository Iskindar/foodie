var app = getApp();
var userINFO;
var display = false;
var Bmob = require('../../utils/bmob.js');
var util = require('../../utils/util.js')
Page({
  data: {
    userInfo: {},
    logs: []
  },
  onLoad: function (options) {

    wx.clearStorageSync("mood");
    var that = this;
    app.getUserInfo(function (userInfo) {
      //更新数据
      userINFO = userInfo;
      that.setData({
        userInfo: userInfo,
      })
    })
  },
  onAddTap: function () {
    wx.navigateTo({
      url: 'add/add',
      success: function (res) {
        // success
      },
      fail: function () {
        // fail
      },
      complete: function () {
        // complete
      }
    })
  },
  onReady: function () {
    // 页面渲染完成
  },
  onShow: function () {
    console.log(userINFO)
    var moodtext = [];
    moodtext.push(wx.getStorageSync('mood'));
    console.log(moodtext);
    if (moodtext == "") {
      display = false;
    }
    else {
      display = true;
    }
    console.log(util.getNowFormatDate())
    var mood = [];
    var mood_one =
      {
        avatarUrl: userINFO.avatarUrl,
        moodtext: moodtext,
        nickname: userINFO.nickName,
        formatTime: util.getNowFormatDate()
      
      };
    mood.push(mood_one);
    console.log(mood);

    this.setData({
      mood: mood,
      display: display,
      
    })

  },
  onHide: function () {
    // 页面隐藏
  },
  onUnload: function () {
    // 页面关闭
  }
})