Page({
  data: {
    index: 0,

    height: [],
    age: [],
    weight: [],
    environment: [
      {
        id: 1000,
        name: '办公室'
      },
      {
        id: 1001,
        name: '粉尘环境'
      },
      {
        id: 1002,
        name: '高温环境'
      },
      {
        id: 1003,
        name: '低温环境'
      },
      {
        id: 1004,
        name: '接触有害物质环境'
      },
      {
        id: 1005,
        name: '噪声与振动环境'
      },
      {
        id: 1006,
        name: '放射性物质环境'
      }
    ],
  },
  bindAgeChange: function (e) {
    this.setData({
      indexAge: e.detail.value,

    })
  },
  bindHeightChange: function (e) {
    this.setData({
      indexHeight: e.detail.value,
    })
  },
  bindWeightChange: function (e) {
    this.setData({
      indexWeight: e.detail.value,
    })
  },
  bindPickerChange: function (e) {
    console.log('picker发送选择改变，携带值为', e.detail.value)
    this.setData({
      index: e.detail.value,
    })
  },
  onRecommendTap: function () {
    wx.redirectTo({
      url: '../recommend',
      success: function (res) {
        // success
      },
      fail: function (res) {
        // fail
      },
      complete: function (res) {
        // complete
      }
    })
  },
  formSubmit: function (e) {
    var orderInfo = e.detail.value;
    try {
      wx.setStorageSync('order', orderInfo)
    } catch (e) {
    }
  },
  onLoad: function (options) {
    for (var i = 0; i < 120; i++) {
      this.data.age[i] = i+1;
    }
    for (var j = 0; j < 130; j++) {
      this.data.height[j] = j+100;
    }
    for (var k = 0; k < 175; k++) {
      this.data.weight[k] = k+15;
    }
    console.log(this.data.age)
    console.log(this.data.height)
    console.log(this.data.weight)
    this.setData({
      age: this.data.age,
      weight: this.data.weight,
      height: this.data.height,
      environment: this.data.environment,
    })
  },

})