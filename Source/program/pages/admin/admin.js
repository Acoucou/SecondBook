// pages/admin/admin.js
const app = getApp().globalData;
let {client, BOOK} = getApp().globalData;

let {
    bookId,
    bookName,
    bookPrice,
    bookCart,
    bookDetail,
    bookPic,
    bookAck
} = getApp().globalData;

Page({

    /**
     * 页面的初始数据
     */
    data: {
        BOOK: {},

        bookId: bookId,
        bookName: bookName,
        bookPrice: bookPrice,
        bookCart: bookCart,
        bookDetail: bookDetail,
        bookPic: [],

        music : "hello"
    },

    /**
     * 生命周期函数--监听页面加载
     */
    onLoad: function (options) {
        this.setData({
            BOOK:BOOK
        })
    },
    ack(e){
        var index = e.currentTarget.dataset.index;
        BOOK[index].bookAck = 1;

        var book = toJson("bookId", bookId[0])
        client.publish('book', book)

        function toJson(key, value){
            var ret = "{\"" + key + "\"" + ":"  + " \"" + value + "\"}"
        
            return ret;
        }
        console.log(ret)
    },

    /**
     * 生命周期函数--监听页面初次渲染完成
     */
    onReady: function () {

    },

    /**
     * 生命周期函数--监听页面显示
     */
    onShow: function () {
        // this.setData({
        //     bookName: app.bookName,
        //     bookPrice: app.bookPrice,
        //     bookCart: app.bookCart,
        //     bookDetail: app.bookDetail,
        //     bookPic: app.bookPic
        // })
        console.log(BOOK)
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
