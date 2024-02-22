// pages/stores/stores.js
const app = getApp().globalData;
// const db = wx.cloud.database().collection("test_db")

let {BOOK, bookAck} = getApp().globalData;

Page({

    /**
     * 页面的初始数据
     */
    data: {
        activeNum: 1,
        BOOK: [],

        bookName: app.bookName,
        bookPrice: app.bookPrice,
        bookCart: app.bookCart,
        bookDetail: app.bookDetail,
        bookPic: [],
        bookAck: app.bookAck,
    },

    /**
     * 生命周期函数--监听页面加载
     */
    onLoad: function (options) {
        // db.get().then(res =>{
        //     //console.log(res.data)
        //     this.setData({
        //         BOOK:res.data
        //     })
        //     console.log(this.data.BOOK)
        // })
        this.setData({
            BOOK: BOOK,
        })
        console.log(BOOK);
    },
    changeTab: function (event) {
        //切换筛选tab
        var num = event.target.dataset.id;
        this.setData({
            activeNum: num
        })
    },

    /**
     * 生命周期函数--监听页面初次渲染完成
     */
    onReady: function () {
        
    },
    onRefresh:function(){
    //导航条加载动画
    wx.showNavigationBarLoading()
    //loading 提示框
    wx.showLoading({
      title: 'Loading...',
    })
    console.log("下拉刷新啦");
    setTimeout(function () {
      wx.hideLoading();
      wx.hideNavigationBarLoading();
      //停止下拉刷新
      wx.stopPullDownRefresh();
    }, 2000)
  },


    /**
     * 生命周期函数--监听页面显示
     */
    onShow: function () {
        this.setData({
            BOOK:this.data.BOOK
        })
        console.log(this.data.BOOK)
    },

    /**
     * 生命周期函数--监听页面隐藏
     */
    onHide: function () {

    },

    /**
     * 生命周期函数--监听页面卸载
     */
    onUnload: function () {

    },

    /**
     * 页面相关事件处理函数--监听用户下拉动作
     */
    onPullDownRefresh: function () {
        this.onRefresh();
    },

    /**
     * 页面上拉触底事件的处理函数
     */
    onReachBottom: function () {

    },

    /**
     * 用户点击右上角分享
     */
    onShareAppMessage: function () {

    }
})