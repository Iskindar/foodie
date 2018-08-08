var Bmob = require('../../../utils/bmob.js');
Page({
  data: {},
  onLoad: function (options) {
   
  },
  onSubmitTap: function (e) {
    var Diary = Bmob.Object.extend("diary");
    var diary = new Diary();
    diary.set("title", "建议");
    diary.set("content", e.detail.value.textarea);
    //添加数据，第一个入口参数是null
    diary.save(null, {
      success: function (result) {
        // 添加成功，返回成功之后的objectId（注意：返回的属性名字是id，不是objectId），你还可以在Bmob的Web管理后台看到对应的数据
        console.log("日记创建成功, objectId:" + result.id);
      },
      error: function (result, error) {
        // 添加失败
        console.log('创建日记失败');

      }
    });
    wx.showModal({
      title: '提示',
      content: '提交成功，是否返回主页',
      showCancel: false,
      success: function (res) {
        if (res.confirm) {

          wx.navigateBack({
            delta: 1
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