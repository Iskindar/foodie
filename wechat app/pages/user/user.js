// pages/user/user.js
var app=getApp();
Page({
  data:{
    userInfo:{}
  },
  onPersonalInfoTap:function(){
    wx.navigateTo({
      url: 'personalInfo/personalInfo',
      success: function(res){
        // success
      },
      fail: function() {
        // fail
      },
      complete: function() {
        // complete
      }
    })
  },
  onBraceletTap:function(){
    wx.navigateTo({
      url: 'bracelet/bracelet',
      success: function(res){
        // success
      },
      fail: function(res) {
        // fail
      },
      complete: function(res) {
        // complete
      }
    })
  },
  onHealthTap:function(){
    wx.navigateTo({
      url: 'health-report/health-report',
      success: function(res){
        // success
      },
      fail: function(res) {
        // fail
      },
      complete: function(res) {
        // complete
      }
    })
  },
  onAboutTap:function(){
    wx.navigateTo({
      url: 'about/about',
      success: function(res){
        // success
      },
      fail: function() {
        // fail
      },
      complete: function() {
        // complete
      }
    })

  },
  onFeedbackTap:function(){
    wx.navigateTo({
      url: 'feedback/feedback',
      success: function(res){
        // success
      },
      fail: function() {
        // fail
      },
      complete: function() {
        // complete
      }
    })
  },
  onLoad:function(options){
    var that=this;
    
    app.getUserInfo(function(userInfo){
      //更新数据
      that.setData({
        userInfo:userInfo
      })
    })
    // 页面初始化 options为页面跳转所带来的参数
  },
  onReady:function(){
    // 页面渲染完成
  },
  onShow:function(){
    // 页面显示
  },
  onHide:function(){
    // 页面隐藏
  },
  onUnload:function(){
    // 页面关闭
  }
})