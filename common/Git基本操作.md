##Git基本操作
git clone 代码路径                    //克隆代码
git pull                              //刷新代码
git status                            //查询代码仓库状态
git diff [filename]                   //查询文件变更信息
git add [filename]|[.]                //添加修改，实际上就是把文件修改添加到暂存区
git commit -m “commit message”        //提交修改，实际上就是把暂存区的所有内容提交到当前分支
git push                              //推送到远端
git log                               //查询修改日志记录
git log --pretty=oneline              //查询修改日志记录，简略信息
git reset --hard HEAD^                //回退到上一个版本
git reset --hard HEAD^^               //回退到上两个版本
git reset --hard HEAD~100             //回退到上100个版本
git reset --hard commitid             //回退到指定版本
git reflog                            //查询历史操作记录
git checkout -- filename              //把文件在工作区的修改全部撤销，包括如下两种情况
									  // | -- 一种是文件修改后还没有被放到暂存区，现在，撤销修改就回到和版本库一模一样的状态
									  // | -- 一种是已经添加到暂存区后，又做了修改，现在，撤销修改就回到添加到暂存区后的状态
git rm file                           //从版本库中删除文件
git branch                            //查看当前分支
git branch dev                        //创建dev分支
git checkout dev                      //切换到dev分支
git checkout -b dev                   //创建并切换到dev分支
git branch -d dev                     //删除dev分支
git merge dev                         //合并dev分支的内容到目的分支，执行该命令前，需先手动执行git branch *切换到目的分支

注：分支提交代码只需要commit，不需要push，因为没有远端分支

####Git配置公钥
git config --global user.name "jyjiangkai"					//配置全局变量用户名
git config --global user.email "jyjiangkai@163.com"			//配置全局变量邮箱地址
git config --list											//检查全局变量是否配置成功
ssh-keygen -t rsa -C "jyjiangkai@163.com"					//生成公钥
cat ~/.ssh/id_rsa.pub										//打开公钥文件，拷贝到工程的`SSH keys`中即生效

 