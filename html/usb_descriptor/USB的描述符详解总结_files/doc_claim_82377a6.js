define("wkview:widget/doc_claim/doc_claim.js",function(s,n,i){var c=s("wkcommon:widget/ui/lib/jquery/jquery.js"),t=s("wkcommon:widget/lib/doT/doT.min.js"),o=s("wkcommon:widget/ui/js_core/_dialog/_dialog.js"),a=s("wkcommon:widget/ui/lib/browser/browser.js"),e=s("wkcommon:widget/ui/js_core/login/login.js"),l=s("wkcommon:widget/ui/js_core/log/log.js"),p=function(s){this.opt=s,this.init()};p.prototype={init:function(){this.bindEvent(),a.ie&&a.ie<=8||s.async("wkcommon:widget/third_party/clipboard/clipboard.js",function(s){this.clipboard=new s(".btn-copy"),this.clipboard.on("success",function(s){s.clearSelection(),c(".success-tip").show(),setTimeout(function(){c(".success-tip").hide()},3e3)})})},showDialog:function(s){{var n=o.customDialog,i=t.template('<div class="claim-wrap">\n    <p class="header">\u8ba4\u9886</p>\n    <div class="btn-close"></div>\n    <div class="btn-close-hover"></div>\n    <div class="cont">\n        <p class="title">\u5982\u60a8\u4e3a\u6b64\u7bc7\u6587\u6863\u4f5c\u8005\uff0c\u53ef\u63d0\u4f9b\u8bc1\u660e\u6750\u6599\u7533\u8bf7\u8ba4\u9886\uff0c\u83b7\u53d6\u7248\u6743\u6587\u6863\u6536\u76ca\u5e76\u4e0b\u67b6\u4fb5\u6743\u6587\u6863\uff1a</p>\n        <div class="step-wrap">\n            <div>\n                <span class="step step1"></span>\n                <p class="desc">\u53d1\u9001\u7533\u8bf7\u90ae\u4ef6</p>\n            </div>\n            <div class="next"></div>\n            <div>\n                <span class="step step2"></span>\n                <p class="desc">\u5ba1\u6838</p>\n                <p class="small-desc">\uff081-2\u4e2a\u5de5\u4f5c\u65e5\uff09</p>\n            </div>\n            <div class="next"></div>\n            <div>\n                <span class="step step3"></span>\n                <p class="desc">\u8ba4\u9886\u7ed3\u679c\u7b54\u590d</p>\n            </div>\n        </div>\n        <p class="title t-color">\u7533\u8bf7\u90ae\u7bb1\uff1a\n            <span id="claim-email">wenku-renling@baidu.com</span>\n            <a class="btn btn-copy" data-clipboard-target="#claim-email">\u590d\u5236</a>\n            <span class="success-tip">\u590d\u5236\u6210\u529f</span>\n        </p>\n        <p class="title t-color mb18">\u8bc1\u660e\u6750\u6599\uff1a</p>\n        <p class="list">1\u3001\u4fb5\u6743\u6587\u6863\u6587\u5e93\u94fe\u63a5\u5730\u5740</p>\n        <p class="list">2\u3001\u60a8\u7684\u7248\u6743\u6587\u6863\u6587\u5e93\u94fe\u63a5\u5730\u5740\uff1a\u5982\u5c1a\u65e0\uff0c\u8bf7\u5148<a href="/new?fr=claim" target="_blank" class="btn btn-upload">\u4e0a\u4f20</a></p>\n        <p class="list">3\u3001\u8eab\u4efd\u8bc1\u660e\u6750\u6599\uff1a\u8eab\u4efd\u8bc1\u6b63\u9762\u7167 / \u62a4\u7167\u8eab\u4efd\u9875\u7167\u7247</p>\n        <p class="list">4\u3001\u7248\u6743\u8bc1\u660e\u6750\u6599\uff1a\u4f5c\u54c1\u8ba4\u8bc1\u8bc1\u4e66 / \u4f5c\u54c1\u767b\u8bb0\u8bc1\u4e66 / \u5df2\u7f72\u540d\u7684\u51fa\u7248\u7269</p>\n        \n    </div>\n    <div class="footer-wrap">\n        <p><span class="at-icon"></span>\u4e13\u5c5e\u5ba2\u670d\uff1aQQ 800049878&nbsp;&nbsp;&nbsp;<span class="tel">\u7535\u8bdd </span>400-921-7005</p>\n    </div>\n</div>');new n({width:680,height:522,content:i({docid:s}),closeSelector:".btn-close"})}},bindEvent:function(){var s=this,n=c(".renlin-wrap");n.length>0&&(n.css("padding-right","10px"),n.siblings(".report").css("margin-left","-10px")),c("body").on("click",".renlin-wrap",function(){return l.xsend(1,102348),s.opt.isLogin?void s.showDialog(s.opt.docId):void e.login.verlogin(function(){})})}},i.exports=p});