// pages/sellingBook/sellingBook.js
const app = getApp().globalData;
// const db = wx.cloud.database().collection("test_db")

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

        array: [' ','课外书', '小说类', '文学类', '儿童读物','手册','笔记'],
        objectArray: [
          {
            id: 0,
            name: '课外书'
          },
          {
            id: 1,
            name: '小说类'
          },
          {
            id: 2,
            name: '文学类'
          },
          {
            id: 3,
            name: '儿童读物'
          },
          {
            id: 4,
            name: '手册'
          },
          {
            id: 5,
            name: '笔记'
          }
        ],
        index: 0,
    },
    setBookName(e) {
        this.data.bookName = e.detail.value;
    },
    setBookPrice(e) {
        this.data.bookPrice = e.detail.value;
    },
    setBookCart(e) {
        this.data.bookCart = e.detail.value;
    },
    setBookDetail(e) {
        this.data.bookDetail = e.detail.value;
    },
    bindPickerChange: function(e) {
        console.log('picker发送选择改变，携带值为', e)
        this.setData({
          index: e.detail.value
        })
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

        // db.add({
        //     data: this.data.book,
        //     success(res) {
        //         console.log("添加成功：", res)
        //     },
        //     fail(res){
        //         console.log(res)
        //     }
        // })
        wx.showModal({
            title: '填写成功',
            content: '该书籍的身份ID: ' + "--"+str + "--是否立即放置书籍",
            success:(res)=> {
                if (res.confirm) {
                    console.log('用户点击确定')
                    // 添加数据
                    this.data.book.bookName = this.data.bookName;
                    this.data.book.bookPrice = this.data.bookPrice;
                    this.data.book.bookCart = this.data.bookCart;
                    this.data.book.bookDetail = this.data.bookDetail;
                    this.data.book.bookId = str;
                    this.data.book.bookPic = bookPic;
                    this.data.book.bookAck = 0;
                    this.data.book.bookIndex = index;
                    index++;
                    BOOK.push(this.data.book)
                    console.log(this.data.book)

                    // 发送放书确认
                    var bookId = "{\"cmd\": " + "\"" + "pushbook" + "\", \"bookId\": " + "\"" + str + "\"}"   
                    client.publish('book', bookId)
                } else if (res.cancel) {
                    // 添加数据
                    this.data.book.bookName = this.data.bookName;
                    this.data.book.bookPrice = this.data.bookPrice;
                    this.data.book.bookCart = this.data.bookCart;
                    this.data.book.bookDetail = this.data.bookDetail;
                    this.data.book.bookId = str;
                    this.data.book.bookPic = bookPic;
                    this.data.book.bookAck = 0;
                    this.data.book.bookIndex = index;
                    index++;
                    BOOK.push(this.data.book)
                    console.log(this.data.book)

                    var bookId = "{\"cmd\": " + "\"" + "addbook" + "\", \"bookId\": " + "\"" + str + "\"}"   
                    client.publish('book', bookId)
                    console.log('用户点击取消')
                }
            }
        })
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
                        bookPic.push(tempFilePaths[i]);
                        cnt++;
                    }
                }     
                that.setData({
                    imgs: imgs,
                });

                // for(var i=0; i<cnt; i++){
                //     wx.cloud.uploadFile({
                //         cloudPath: "my_test/" + new Date().getTime() + '.jpg',
                //         filePath: imgs[i],
                //         success(res){
                //             bookPic.push(res.fileID)
                //             console.log("上传成功", res)
                //         }
                //     })
                    
                // }  
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