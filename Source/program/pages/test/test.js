// pages/getQrCode/getQrCode.js
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
        img:[]
    },
  
    /**
     * 生命周期函数--监听页面加载
     */
    onLoad: function (options) {
        
        let that = this;
        const APP_ID = 'wxe20f907ffb1a1705';// 小程序appid
        const APP_SECRET = '9f84b87ec06d13f4751194b3fbf50caf';// 小程序app_secret
        let access_token = '';
  
        wx.request({
            url:"https://api.weixin.qq.com/cgi-bin/token",
            data: {
                grant_type: 'client_credential',
                appid: APP_ID,
                secret: APP_SECRET
            },
            success:function(res){
                access_token = res.data.access_token;
                console.log(res);
                console.log(access_token);
  
                // 接口A：适用于需要的码数量较少的业务场景 生成的是小程序码
                //that.getQrCode_A(access_token);
  
                // 接口B：适用于需要的码数量极多的业务场景 生成的是小程序码
                //that.getQrCode_B(access_token);
  
                // 接口C：适用于需要的码数量较少的业务场景 生成的是小程序二维码
                that.getQrCode_C(access_token);
            }
        })
    },
    test(e) {
        console.log(client)
        client.publish('book', "fsdafa");
    },
  
      /**
       * 接口A：适用于需要的码数量较少的业务场景
       * @param access_token
       */
    //   getQrCode_A: function (access_token) {
    //       wx.request({
    //           url:"https://api.weixin.qq.com/wxa/getwxacode?access_token=" + access_token,
    //           method: 'POST',
    //           data: {
    //               path: 'pages/index/index', // 必须是已经发布的小程序存在的页面（否则报错）
    //               width: 430,
    //               auto_color: false,// 自动配置线条颜色，如果颜色依然是黑色，则说明不建议配置主色调
    //               line_color: {"r":"0","g":"0","b":"0"} // auto_color 为 false 时生效，使用 rgb 设置颜色 例如 {"r":"xxx","g":"xxx","b":"xxx"} 十进制表示
    //           },
    //           success:function(res){
    //               console.log(res);
    //           }
    //       })
    //   },
  
      /**
       * 接口B：适用于需要的码数量极多的业务场景
       * @param access_token
       */
    //   getQrCode_B: function (access_token) {
    //       wx.request({
    //           url:"https://api.weixin.qq.com/wxa/getwxacodeunlimit?access_token=" + access_token,
    //           method: 'POST',
    //           data: {
    //               scene: '200',
    //               page: 'pages/index/index', // 必须是已经发布的小程序存在的页面（否则报错）
    //               width: 430,
    //               auto_color: false,// 自动配置线条颜色，如果颜色依然是黑色，则说明不建议配置主色调
    //               line_color: {"r":"0","g":"0","b":"0"} // auto_color 为 false 时生效，使用 rgb 设置颜色 例如 {"r":"xxx","g":"xxx","b":"xxx"} 十进制表示
    //           },
    //           success:function(res){
    //               console.log(res);
    //           }
    //       })
    //   },
  
      /**
       * 接口C：适用于需要的码数量较少的业务场景 生成小程序二维码
       * @param access_token
       */
      getQrCode_C: function (access_token) {
          let that = this;
          wx.request({
              url:"https://api.weixin.qq.com/cgi-bin/wxaapp/createwxaqrcode?access_token=" + access_token,
              method: 'POST',
              responseType:"arraybuffer",
              data: {
                  path: 'pages/index/index',
                  width: 430,
              },
              success:function(res){
                  console.log("fdsf\n")
                  console.log(res);

                  that.setData({
                      img:wx.arrayBufferToBase64(res.data)
                  })
              }
          })
      }
  });