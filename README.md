# MinesweeperRobot (扫雷机器人)
A Windows 7 minesweeper robot

扫雷机器人的用途当然就是自动扫雷了，需要 Windows 7 系统的扫雷（Vista 的扫雷可能也可以，因为好像和 Win 7 的长的一样，没测试过）。使用方法是打开扫雷与扫雷机器人，在扫雷中设置好你想玩的难度，并使扫雷机器人中的难度设置保持一致。扫雷机器人中的动画设置也必须与扫雷一致。在扫雷机器人中点击“开始新局”就开始自动扫雷了。

![Snapshot](https://raw.githubusercontent.com/kfstorm/MinesweeperRobot/master/github/snapshot.jpg)

![Animation](https://raw.githubusercontent.com/kfstorm/MinesweeperRobot/master/github/MinesweeperRobot_Animation.gif)

# 使用注意
- 扫雷必须使用默认的蓝色方块样式。
- 点击“开始新局”时扫雷必须处
- 于刚开始一个新局的状态。（不能是重玩状态，不能打开设置窗口）
- 自动扫雷时不要乱动鼠标。
- 有极小几率识别错误。

# 原理
截取图像识别扫雷窗口中各个方块的数字（不是偷看内存哦，不是作弊哦），并利用我的算法决定哪些方块可以安全点开（我可以自信的说，这个算法可以正确点开绝大多数人不敢点开的方块）。