var util = require("../../../utils/util.js");
var wxCharts = require('../../../utils/wxcharts.js');
var recommendData = require('../../../data/data_base.js');
var foodData = require('../../../data/data_base.js');
var mainElementData = require('../../../data/data_base.js');
var app = getApp();
var rf =false;
var foodId = [1,2,3];
Page({
  data: {
    modalHidden: true,
    receive:false
  },
  
  processFoodData: function (foodData, settedKey) {
    var totalCalorie = 0;
    var intakeCarbohydrates = 0;
    var intakeProteins = 0;
    var intakeFats = 0;
    var intakeNa = 0;
    var intakeCa = 0;
    var intakeFe = 0;
    var intakeK = 0;
    var intakeZn = 0;
    var intakeSe = 0;
    var intakeVA = 0;
    var intakeVB1 = 0;
    var intakeVB2 = 0;
    var intakeVC = 0;
    var intakeVE = 0;
    var intakeDietaryFiber = 0;
    var intakeNiacin = 0;
    var intakeCholesterol = 0;
    var intakeMg = 0;
    var intakeCu = 0;
    var intakeMn = 0;
    //出现相同食物的情况
    // var samefood=[{
    //     foodNum:0,
    //     sameId:0,
    // }];

    // var count=0;

    // var foodFalseId=[].concat(foodId);
    // for (var i=0;i<foodId.length;i++){
    //     for(var j=0;j<foodFalseId.length;j++){
    //         if(foodId[i]==foodFalseId[j])
    //         {

    //             samefood.foodNum[count]++;
    //             samefood.sameId[count]=foodId[i];
    //             count++;
    //         }      
    //     }
    // }
    // console.log(samefood);
    var food = [];
   
    for (var idx in foodData.foodList) {

      var singleSubject = foodData.foodList[idx];
      for (var j = 0; j < foodId.length; j++) {
        if (foodId[j] == singleSubject.foodId) {
          totalCalorie += singleSubject.calorie;
          intakeCarbohydrates += singleSubject.carbohydrates;
          intakeProteins += singleSubject.proteins;
          intakeFats += singleSubject.fats;
          intakeNa += singleSubject.Na;
          intakeCa += singleSubject.Ca;
          intakeFe += singleSubject.Fe;
          intakeK += singleSubject.K;
          intakeZn += singleSubject.Zn;
          intakeSe += singleSubject.Se;
          intakeVA += singleSubject.VA;
          intakeVB1 += singleSubject.VB1;
          intakeVB2 += singleSubject.VB2;
          intakeVC += singleSubject.VC;
          intakeVE += singleSubject.VE;
          intakeDietaryFiber += singleSubject.dietaryFiber;
          intakeNiacin += singleSubject.niacin;
          intakeCholesterol += singleSubject.cholesterol;
          intakeMg += singleSubject.Mg;
          intakeCu += singleSubject.Cu;
          intakeMn += singleSubject.Mn;
          var temp1 = {
            name: singleSubject.name,
            calorie: singleSubject.calorie,
            foodId: singleSubject.foodId
          }
          food.push(temp1)
        }
      }
    }
    var mainElement = [intakeCarbohydrates, intakeProteins, intakeFats];
    var microElement = [
      intakeNa,
      intakeCa,
      intakeFe,
      intakeK,
      intakeZn,
      intakeSe,
      intakeVA,
      intakeVB1,
      intakeVB2,
      intakeVC,
      intakeVE,
      intakeDietaryFiber,
      intakeNiacin,
      intakeCholesterol,
      intakeMg,
      intakeCu,
      intakeMn,
    ];
    for (var i = 0; i < microElement.length; i++) {
      microElement[i] = util.returnFloat(microElement[i]);
    }

    this.setData({
      food: food,
      totalCalorie: totalCalorie,

    })
    if (settedKey == "microElement") {
      return microElement;
    }
    else if (settedKey == "mainElement") {
      return mainElement;
    }
    else if (settedKey == "totalCalorie") {
      return totalCalorie;
    }

  },
  onLoad: function (e) {
   
    //画饼图
    var windowWidth = 320;
    try {
      var res = wx.getSystemInfoSync();
      windowWidth = res.windowWidth;
    } catch (e) {
      console.error('getSystemInfoSync failed!');
    }

    new wxCharts({
      animation: true,
      canvasId: 'pieCanvas',
      type: 'pie',
      series: [{
        name: '碳水化合物',
        data: this.processFoodData(foodData, "mainElement")[0],
      }, {
        name: '脂肪',
        data: this.processFoodData(foodData, "mainElement")[1],
      }, {
        name: '蛋白质',
        data: this.processFoodData(foodData, "mainElement")[2],
      }],
      width: windowWidth,
      height: 300,
      dataLabel: true,
    });
    var mainJudge = [];
    var mainPercent = [];
    var dataUp = [0.65, 0.30, 0.15];
    var dataDown = [0.55, 0.20, 0.10];
    for (var i = 0; i < 3; i++) {

      mainPercent[i] = this.processFoodData(foodData, "mainElement")[i] / (this.processFoodData(foodData, "mainElement")[0] + this.processFoodData(foodData, "mainElement")[1] + this.processFoodData(foodData, "mainElement")[2]);
      if (mainPercent[i] > dataUp[i]) {
        mainJudge[i] = 2;
      }
      else {
        if (mainPercent[0] < dataDown[i]) {
          mainJudge[i] = 0;
        }
        else {
          mainJudge[i] = 1;
        }
      }

    }

    //三大营养素的数据绑定
    var mainElement = [];
    var array = [];

    for (var i in mainElementData.mainElementList) {

      var subject = mainElementData.mainElementList[i];

      for (var j = 0; j < 3; j++) {
        array[j] = this.processFoodData(foodData, "mainElement")[j];
        array[j] = util.returnFloat(array[j]);
      }
      var temp = {
        name: subject.name,
        imageUrl: subject.imageUrl,
        weight: array[i],
        mainJudge: mainJudge[i]
      }
      mainElement.push(temp);
    }
    //能量scoreEnergy的计算
    var scoreEnergy = 0;
    var energyRNI = 2100*0.3;
    var energyLTI = ((energyRNI - 5) / 1.16) * 0.68 + 5;
    var calorieIntake = this.processFoodData(foodData, "totalCalorie");
    if (calorieIntake < 1913) {
      scoreEnergy = util.calUx(calorieIntake, energyRNI, energyLTI) * 100;
    } else if (calorieIntake > 2870) {
      scoreEnergy = util.calUx(calorieIntake, energyRNI, energyLTI) * 100;
    } else {
      scoreEnergy = 100;
    }

    //备注百分比的计算 
    var me = [];
    var microPercent = [];
    var microJudge = [];
    var Sij = [];
    var absSij = [];
    var DQD = 0;
    //营养评价总分的计算
    var totalScore = scoreEnergy * 0.26 + 0.74 * scoreDBI;


    for (var idx in recommendData.recommendList) {
      var subject = recommendData.recommendList[idx];
      subject.intake = this.processFoodData(foodData, "microElement")[idx];
      microPercent[idx] = (subject.intake / subject.recommend);
      if (subject.intake > 0.8 * subject.recommend && subject.intake < 1.2 * subject.recommend) {
        Sij[idx] = 0;
        absSij[idx] = 0;
      } else if (subject.intake < 0.8 * subject.recommend && subject.intake > 0.7 * subject.recommend) {
        Sij[idx] = -1;
        absSij[idx] = 1;
      } else if (subject.intake < 1.3 * subject.recommend && subject.intake > 1.2 * subject.recommend) {
        Sij[idx] = 1;
        absSij[idx] = 1;
      } else if (subject.intake < 0.7 * subject.recommend) {
        Sij[idx] = -3;
        absSij[idx] = 3;
      } else {
        Sij[idx] = 3;
        absSij[idx] = 3;
      }
      DQD += absSij[idx];

      //判断颜色
      if ((microPercent[idx] < (1.2)) && (microPercent[idx] > 0.8)) {
        microJudge[idx] = 1;

      }
      else {
        microJudge[idx] = 0;
      }
      var temp = {
        name: subject.name,
        recommend: subject.recommend,
        recommendText: subject.recommendText,
        intake: subject.intake,
        multiple: (microPercent[idx] * 100).toPrecision(3),
        suggestion: subject.suggestion,
        modalHidden: true,
        microJudge: microJudge[idx]
      }
      me.push(temp);
    }
    console.log(me);
    //营养素scoreDBI的计算
   
    var scoreDBI = 100 - 1.19 * DQD;
    totalScore = 0.26 * scoreEnergy + 0.74 * scoreDBI;
    var percent = totalScore * 0.02 - 0.5;
  
    totalScore = totalScore.toPrecision(2);
    this.setData({
      recommend_key: me,
    })
    console.log(util.getNowFormatDate());

    //圆形进度条的实现
    util.makeArc(percent, '#d2d2d2', '#d81e06');
    //数据绑定
    this.setData({
      score: totalScore,
    })
    this.setData({
      mainElement: mainElement,
      formatDate: util.getNowFormatDate(),
    })
  },
  onShow: function () {
    var that = this;
    wx.request({
      url: 'http://123.207.98.36:5000/get_information', //仅为示例，并非真实的接口地址
      data: {
        id: '',

      },
      method: 'GET',
      header: {
        'content-type': 'application/json'
      },
      success: function (res) {
        console.log(res.data)
        if (res.data == -1) {
          that.setData({
            receive: false,
          })
        }
        else
          that.setData({
            receive: true,
          })
      },
      fail: function () {
        console.log('error')
      },
    })
    wx.stopPullDownRefresh();
  },
  
})
