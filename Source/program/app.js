var mqtt = require('mqtt/mqtt.min.js');

App({
  globalData: {
    mybookList:[],  // 我的书籍
    BOOK: [],  // 书籍对象
    index: 2,  // 书籍指针

    bookId: [],
    bookName: [],
    bookPrice: [],
    bookCart: [],
    bookDetail: [],
    bookPic: [],
    bookAck: [], // 书籍审核响应

    client : null,
    topic: ["book", "connectState"],
  },

  /**
   * 当小程序初始化完成时，会触发 onLaunch（全局只触发一次）
   */
  onLaunch: function () {
    // wx.cloud.init({
    //     env:"cloud1-5gffyu7d7a7a5a08"
    // })

    this.connectMqtt();

    var book = {};
    book.bookName = "HTML 笔记";
    book.bookPrice = "5";
    book.bookCart = "资料";
    book.bookDetail = "这是我写的HTML笔记，要毕业了，卖给有需要的人，里面列出了详细各个标签作用以及功能";
    book.bookId = "0123456789";
    book.bookPic = ["/image/1.jpg", "/image/登录注册.png"];
    book.bookAck = "0";
    book.bookIndex = 1;

    this.globalData.BOOK.push(book);
  },
  connectMqtt() {
    let that = this;
    // 配置与连接
    const options = {
      connectTimeout: 4000, // 超时时间
      clientId: 'wx_' + parseInt(Math.random() * 100 + 800, 10),
      port: 8083, //重点注意这个,坑了我很久
      // username: 'xxx',
      // password: 'xxx',
    }
    that.globalData.client = mqtt.connect('wx://81.71.154.91/mqtt', options) // 连接
    that.globalData.client.on('reconnect', (error) => { // 回调函数
      console.log('正在重连:', error)
    })
    that.globalData.client.on('error', (error) => {
      console.log('连接失败:', error)
    })

    // 成功的回调函数
    that.globalData.client.on('connect', (e) => {
      console.log('成功连接服务器')
      // 订阅主题
      that.globalData.client.subscribe(that.globalData.topic, {
        qos: 0
      }, function (err) {
        if (!err) {
          console.log("主题订阅成功: " + that.globalData.topic)
        }
      })
    })

    // 处理接收到的消息
    that.globalData.client.on('message', function (topic, message) {
        var str = message.toString();
      console.log("接收到消息： "+topic + " : " + str);

      // 检测是否连接
      if (topic == "book") {
        var id = str.slice(str.indexOf("bookId")+9, str.indexOf("bookId")+19);
        var statu = str.slice(str.indexOf("bookAck")+11, str.indexOf("bookAck")+12);

        for (let index = 0; index < that.globalData.BOOK.length; index++) {
            if (id == that.globalData.BOOK[index].bookId) {
                if (statu === '1') {
                    that.globalData.BOOK[index].bookAck = 1;
                    console.log("卖书");
                } 
                 else if(statu === '2'){
                    // 预览书
                    that.globalData.BOOK[index].bookAck = 1;
                    console.log("预览书");
                }
                else {
                    that.globalData.BOOK[index].bookAck = 0;
                    console.log("0000");
                }
                break;
            }
        }

        console.log("id:" + id +"    " + "statu: " + statu);
      }
    })
  },
  

  /**
   * 当小程序启动，或从后台进入前台显示，会触发 onShow
   */
  onShow: function (options) {

  },

  /**
   * 当小程序从前台进入后台，会触发 onHide
   */
  onHide: function () {

  },

  /**
   * 当小程序发生脚本错误，或者 api 调用失败时，会触发 onError 并带上错误信息
   */
  onError: function (msg) {

  }
})
