// const db = wx.cloud.database().collection("test_db")

let {
    bookId,
    bookName,
    bookPrice,
    bookCart,
    bookDetail,
    bookPic,
    BOOK,
    client
} = getApp().globalData;
Page({

    /**
     * 页面的初始数据
     */
    data: {
        goodsObj: {},
        // 商品是否被收藏
        isCollect: false,

        BOOK: [],
        index: 0
    },
    // 商品对象
    GoodsInfo: {},
    /**
     * 生命周期函数--监听页面加载
     */
    onLoad:function(e){
        // db.get().then(res =>{
        //     //console.log(res.data)
        //     this.setData({
        //         BOOK:res.data
        //     })
        // })
        //console.log(BOOK)
        //console.log(e.index)
        this.setData({
            BOOK: BOOK,
            index: e.index
        })
    },
    onShow: function () {
        let pages = getCurrentPages();
        let currentPage = pages[pages.length - 1];
        let options = currentPage.options;
        const {
            goods_id
        } = options;
        // this.getGoodsDetail(goods_id);
    },
    // 点击轮播图 放大预览
    handlePrevewImage(e) {
        // 1 先构造要预览的图片数组 
        let index = e.currentTarget.dataset.index;
        const urls = bookPic;
        wx.previewImage({
            current: bookPic[index],
            urls
        });
    },
    // 点击 加入购物车
    handleCartAdd() {
        // 1 获取缓存中的购物车 数组
        let cart = wx.getStorageSync("cart") || [];
        // 2 判断 商品对象是否存在于购物车数组中
        let index = cart.findIndex(v => v.goods_id === this.GoodsInfo.goods_id);
        if (index === -1) {
            //3  不存在 第一次添加
            this.GoodsInfo.num = 1;
            this.GoodsInfo.checked = true;
            cart.push(this.GoodsInfo);
        } else {
            // 4 已经存在购物车数据 执行 num++
            cart[index].num++;
        }
        // 5 把购物车重新添加回缓存中
        wx.setStorageSync("cart", cart);
        // 6 弹窗提示
        wx.showToast({
            title: '加入成功',
            icon: 'success',
            // true 防止用户 手抖 疯狂点击按钮 
            mask: true
        });
    },
    //  购买书籍
    buyingBook(e) {
        wx.showModal({
            title: '购买书籍',
            content: '该书籍的身份ID: ' +"--"+ this.data.BOOK[this.data.index].bookId + "--是否立即取书",
            success:(res)=> {
                if (res.confirm) {
                    console.log('用户点击确定')

                    var bookId = "{\"cmd\": " + "\"" + "removebook" + "\", \"bookId\": " + "\"" + this.data.BOOK[this.data.index].bookId + "\"}"; 
                    client.publish('book', bookId);

                } else if (res.cancel) {
                    console.log('用户点击取消')
                    var bookId = "{\"cmd\": " + "\"" + "delbook" + "\", \"bookId\": " + "\"" + this.data.BOOK[this.data.index].bookId + "\"}"; 
                    client.publish('book', bookId);
                }
            },
            fail(res){
                console.log("shibai")
            }
        })
    },
    //  预览书籍
    LookingBook(e) {
        wx.showModal({
            title: '',
            content: '该书籍的身份ID: ' +"--"+ this.data.BOOK[this.data.index].bookId + "--是否立即取书预览",
            success:(res)=> {
                if (res.confirm) {
                    console.log('用户点击确定')

                    var bookId = "{\"cmd\": " + "\"" + "prebook" + "\", \"bookId\": " + "\"" + this.data.BOOK[this.data.index].bookId + "\"}"; 
                    client.publish('book', bookId);
                } else if (res.cancel) {
                    console.log('用户点击取消')
                    var bookId = "{\"cmd\": " + "\"" + "prebook" + "\", \"bookId\": " + "\"" + this.data.BOOK[this.data.index].bookId + "\"}"; 
                    client.publish('book', bookId);
                }
            },
            fail(res){
                console.log("shibai")
            }
        })
    },
    //  归还书籍
    ReturnBook(e) {
        wx.showModal({
            title: '归还书籍',
            content: '该书籍的身份ID: ' +"--"+ this.data.BOOK[this.data.index].bookId + "--是否立即归还书籍",
            success:(res)=> {
                if (res.confirm) {
                    console.log('用户点击确定')

                    var bookId = "{\"cmd\": " + "\"" + "returnbook" + "\", \"bookId\": " + "\"" + this.data.BOOK[this.data.index].bookId + "\"}"; 
                    client.publish('book', bookId);

                } else if (res.cancel) {
                    console.log('用户点击取消')
                    // var bookId = "{\"cmd\": " + "\"" + "delbook" + "\", \"bookId\": " + "\"" + this.data.BOOK[this.data.index].bookId + "\"}"; 
                    // client.publish('book', bookId);
                }
            },
            fail(res){
                console.log("shibai")
            }
        })
    },
    // 点击 商品收藏图标
    handleCollect() {
        let isCollect = false;
        // 1 获取缓存中的商品收藏数组
        let collect = wx.getStorageSync("collect") || [];
        // 2 判断该商品是否被收藏过
        let index = collect.findIndex(v => v.goods_id === this.GoodsInfo.goods_id);
        // 3 当index！=-1表示 已经收藏过 
        if (index !== -1) {
            // 能找到 已经收藏过了  在数组中删除该商品
            collect.splice(index, 1);
            isCollect = false;
            wx.showToast({
                title: '取消成功',
                icon: 'success',
                mask: true
            });

        } else {
            // 没有收藏过
            collect.push(this.GoodsInfo);
            isCollect = true;
            wx.showToast({
                title: '收藏成功',
                icon: 'success',
                mask: true
            });
        }
        // 4 把数组存入到缓存中
        wx.setStorageSync("collect", collect);
        // 5 修改data中的属性  isCollect
        this.setData({
            isCollect
        })
    }

})