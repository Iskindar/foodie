var discountData = require('../../../data/posts-data.js');
Page({
  data: {},
  onLoad: function (options) {
    var canteen1 = [];
    var canteen4 = [];
    for (var i in discountData.discountList) {
      var subject = discountData.discountList[i];
      if (i < 3) {
        var temp1 = {
          name: subject.name,
          image: subject.image,
          price: subject.price
        };
        canteen1.push(temp1);
      }
      if (i > 1) {
        var temp4 = {
          name: subject.name,
          image: subject.image,
          price: subject.price
        };
        canteen4.push(temp4);
      }
    }
    console.log(canteen1);
    console.log(canteen4);
    this.setData(
      {
        canteen1: canteen1,
        canteen4: canteen4
      }
    );
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