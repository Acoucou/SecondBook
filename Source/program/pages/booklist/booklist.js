let {
    bookId,
    bookName,
    bookPrice,
    bookCart,
    bookDetail,
    bookPic,
    BOOK,
    index,
    client
} = getApp().globalData;

// pages/booklist/booklist.js
Page({

    /**
     * 页面的初始数据
     */
    data: {
        activeNum: 1,
        BOOK: []
    },

    /**
     * 生命周期函数--监听页面加载
     */
    onLoad(options) {
        this.setData({
            BOOK: BOOK,
        })
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
    onReady() {

    },

    onRefresh:function(){
        //导航条加载动画
        wx.showNavigationBarLoading()
        //loading 提示框
        wx.showLoading({
          title: '加载中...',
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
    onShow() {
        this.setData({
            BOOK:this.data.BOOK
        })
    },

    /**
     * 生命周期函数--监听页面隐藏
     */
    onHide() {

    },

    /**
     * 生命周期函数--监听页面卸载
     */
    onUnload() {

    },

    /**
     * 页面相关事件处理函数--监听用户下拉动作
     */
    onPullDownRefresh() {
         this.onRefresh();

    },

    /**
     * 页面上拉触底事件的处理函数
     */
    onReachBottom() {

    },

    /**
     * 用户点击右上角分享
     */
    onShareAppMessage() {

    }
})