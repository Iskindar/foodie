var app = getApp();
var postsData = require('../../data/posts-data.js');
var that;
var Bmob = require('../../utils/bmob.js');
var common = require('../../utils/common.js');
var temp=[];
var openId;
var temp1;
Page({
  data:{
    userInfo:{},
    loading:true
  },
  onLoad:function(options){
   that = this
    //调用应用实例的方法获取全局数据
    app.getUserInfo(function(userInfo){
      temp.push(userInfo);
     
      //更新数据
      that.setData({
        userInfo:userInfo
      })
    })
    console.log(temp)
    wx.login({
      success: function (res) {
        if (res.code) {
          //发起网络请求
          console.log(res.code)

          Bmob.User.requestOpenId(res.code, {
            success: function (result) {
              that.setData({
                loading: true,
                url: result.openid
              })
            },
            error: function (error) {
              // Show the error message somewhere
              console.log("Error: " + error.code + " " + error.message);
            }
          });
        } else {
          console.log('获取用户登录态失败！' + res.errMsg)
          common.showTip('获取用户登录态失败！', 'loading');
        }
      }
    });
 
    console.log(temp1);
    //  wx.request({
    //   url: 'http://123.207.98.36', //仅为示例，并非真实的接口地址
    //   data: {
    //     userinfo:temp[0]
    //   },
    //   method:'POST',
    //   header: {
    //     'content-type': 'application/json'
    //   },
    //   success: function (res) {
    //     console.log(res.data)
    //   }
    // })
    // this.setData({
    //   post_key: postsData.postList
    // });
  },
  onRestInquiryTap:function(){
    wx.navigateTo({
      url: 'rest-inquiry/rest-inquiry',
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
  onStopTap:function(){
    wx.navigateTo({
      url: 'stop/stop',
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
  onRecommendTap:function(){
    wx.navigateTo({
      url: 'recommend/survey/survey',
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
  onReportTap:function(){
    wx.navigateTo({
      url: 'report/report',
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
  onAnalysisTap:function(){
    wx.navigateTo({
      url: 'analysis/analysis',
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
  onDiscountPostTap:function(){
    wx.navigateTo({
      url: 'discount-post/discount-post',
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
  }
})