var mood_text = '';
var Bmob = require('../../../utils/bmob.js');
Page({
  data: {},
  onLoad: function (options) {
  
  },
  formSubmit: function (e) {
    mood_text=e.detail.value.moodText;
    
    var mood = Bmob.Object.extend("mood");
    var mood = new mood();
    mood.set("title", "动态");
    mood.set("content", mood_text);
  
    //添加数据，第一个入口参数是null
    mood.save(null, {
      success: function (result) {
        // 添加成功，返回成功之后的objectId（注意：返回的属性名字是id，不是objectId），你还可以在Bmob的Web管理后台看到对应的数据
        console.log("日记创建成功, objectId:" + result.get('content'));
        wx.setStorageSync('mood', result.get('content'));
      },
      error: function (result, error) {
        // 添加失败
        console.log('创建日记失败');
      }
    });
    
    console.log(mood_text);
  },
  onSendTap: function () {
    wx.showToast({
      title: '发布成功',
      icon: 'success',
      duration: 2000
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