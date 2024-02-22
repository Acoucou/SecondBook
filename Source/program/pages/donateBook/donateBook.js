// pages/sellingBook/sellingBook.js
const app = getApp().globalData;
// const db = wx.cloud.database().collection("donate_book")

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

Page({

    /**
     * 页面的初始数据
     */
    data: {
        book:{}, // book对象

        bookName: "",
        bookPrice: "",
        bookCart: "",
        bookDetail: "",
        bookPic: [],
        imgs: [],
    },
    setBookDetail(e) {
        this.data.bookDetail = e.detail.value;
    },
    sure(e) {
        // 定义存放生成随机数的数组
        var array = new Array();
        // 循环N次生成随机数
        for (var i = 0;; i++) {
            // 只生成10个随机数
            if (array.length < 10) {
                generateRandom(10);
            } else {
                break;
            }
        }
        // 生成随机数的方法
        function generateRandom(count) {
            var rand = parseInt(Math.random() * count);
            for (var i = 0; i < array.length; i++) {
                if (array[i] == rand) {
                    return false;
                }
            }
            array.push(rand);
        }
        var str = array.join("");
        //bookId.push(str);

        this.data.book.bookName = this.data.bookName;
        this.data.book.bookPrice = this.data.bookPrice;
        this.data.book.bookCart = this.data.bookCart;
        this.data.book.bookDetail = this.data.bookDetail;
        this.data.book.bookId = str;
        this.data.book.bookPic = bookPic;
        this.data.book.bookAck = 1;
        this.data.book.bookIndex = index;
        index++;
        BOOK.push(this.data.book)
        console.log(this.data.book)

        // db.add({
        //     data: this.data.book,
        //     success(res) {
        //         console.log("添加成功：", res)
        //     },
        //     fail(res){
        //         console.log(res)
        //     }
        // })

        var bookId = "{\"bookId\": " + "\"" + str + "\"}"
        client.publish('book', bookId)
    },
    /**
     * 生命周期函数--监听页面加载
     */
    onLoad: function (options) {

    },

    /**
     * 生命周期函数--监听页面初次渲染完成
     */
    onReady: function () {

    },
    // 上传图片
    chooseImg: function (e) {
        var cnt = 0;
        var that = this;
        var imgs = this.data.imgs;
        bookPic =  [];

        if (imgs.length >= 9) {
            this.setData({
                lenMore: 1
            });
            setTimeout(function () {
                that.setData({
                    lenMore: 0
                });
            }, 2500);
            return false;
        }
        wx.chooseImage({
            // count: 1, // 默认9
            sizeType: ['original', 'compressed'], // 可以指定是原图还是压缩图，默认二者都有
            sourceType: ['album', 'camera'], // 可以指定来源是相册还是相机，默认二者都有
            success: function (res) {
                // 返回选定照片的本地文件路径列表，tempFilePath可以作为img标签的src属性显示图片
                var tempFilePaths = res.tempFilePaths;
                var imgs = that.data.imgs;
                // console.log(tempFilePaths + '----');
                for (var i = 0; i < tempFilePaths.length; i++) {
                    if (imgs.length >= 9) {
                        that.setData({
                            imgs: imgs
                        });
                        return false;
                    } else {
                        imgs.push(tempFilePaths[i]);
                        //bookPic.push(tempFilePaths[i]);
                        cnt++;
                    }
                }     
                that.setData({
                    imgs: imgs,
                });

                for(var i=0; i<cnt; i++){
                    wx.cloud.uploadFile({
                        cloudPath: "donate_book/" + new Date().getTime() + '.jpg',
                        filePath: imgs[i],
                        success(res){
                            bookPic.push(res.fileID)
                            console.log("上传成功", res)
                            console.log(bookPic)
                        }
                    })
                    
                }  
            }
        });
    },
    // 删除图片
    deleteImg: function (e) {
        var imgs = this.data.imgs;
        var index = e.currentTarget.dataset.index;
        imgs.splice(index, 1);
        this.setData({
            imgs: imgs
        });
    },
    // 预览图片
    previewImg: function (e) {
        //获取当前图片的下标
        var index = e.currentTarget.dataset.index;
        //所有图片
        var imgs = this.data.imgs;
        wx.previewImage({
            //当前显示图片
            current: imgs[index],
            //所有图片
            urls: imgs
        })
    },

    /**
     * 生命周期函数--监听页面显示
     */
    onShow: function () {

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