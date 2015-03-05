package findPath
{
	import face2wind.lib.ObjectPool;
	
	import flash.geom.Point;
	import flash.utils.Dictionary;
	
	/**
	 * 避开怪物追杀的寻路器
	 * @author face2wind
	 */
	public class AvoidMonsterPathFinder extends AStarPathFinder
	{
		public function AvoidMonsterPathFinder()
		{
		}
		
		/**
		 *  寻找最佳的逃生目标点
		 * @param playerPoint 玩家当前所在的点
		 * @return 目的点
		 */		
		public function findEscapePoint(playerPoint:Point, monsterList:Dictionary):Point
		{
			if(AStarPathFinder.canNotMovingArea(_mapData[playerPoint.x][playerPoint.y]))
				return null;
			var monsterDic:Dictionary = new Dictionary(); // 把上面以怪物ID为Key的dic转成以坐标为key的dic
			var down:int = -1; // 记录每个方向能走的步数，默认无限
			var up:int = -1;
			var left:int = -1;
			var right:int = -1;
			for each (var mObj:Object in monsterList) {
				monsterDic[mObj.x+"_"+mObj.y] = mObj;
				if(mObj.x == playerPoint.x){
					if(mObj.y > playerPoint.y)
						down = (mObj.y-playerPoint.y)/3; // 不要走太近怪物身边
					else
						up = (playerPoint.y-mObj.y)/3;
				}
				if(mObj.y == playerPoint.y){
					if(mObj.x > playerPoint.x)
						right =  (mObj.x-playerPoint.x)/3;
					else
						left = (playerPoint.x-mObj.x)/3;
				}
			}
			var step:int = 1;
			var closeList:Dictionary = new Dictionary(); //关闭列表，不再检测
			var checkStack:Array = []; // 待检查列表
			var finalPoints:Array = []; // 最终可行的点列表
			while(true){
				var checking:Boolean = false;
				if(step <= right || mapWidth > (playerPoint.x+step)){
					if(canMovingArea(monsterDic[(playerPoint.x+step).toString()+"_"+playerPoint.y])){
						checkStack.push(new Point(playerPoint.x+step, playerPoint.y));
						finalPoints.push({xxx:xxx});
					}
					closeList[(playerPoint.x+step).toString()+"_"+playerPoint.y] = true;
					checking = true;
				}
				if(step <= left && -1 < (playerPoint.x-step)){
					if(canMovingArea(monsterDic[(playerPoint.x-step).toString()+"_"+playerPoint.y])){
						checkStack.push(new Point(playerPoint.x-step, playerPoint.y));
					}
					closeList[(playerPoint.x-step).toString()+"_"+playerPoint.y] = true;
					checking = true;
				}
				if(step <= down && -1 < (playerPoint.y+step)){
					if(canMovingArea(monsterDic[playerPoint.x+"_"+(playerPoint.y+step).toString()])){
						checkStack.push(new Point(playerPoint.x, playerPoint.y+step));
					}
					closeList[playerPoint.x+"_"+(playerPoint.y+step).toString()] = true;
					checking = true;
				}
				if(step <= up && -1 < (playerPoint.y-step)){
					if(canMovingArea(monsterDic[playerPoint.x+"_"+(playerPoint.y-step).toString()])){
						checkStack.push(new Point(playerPoint.x, playerPoint.y-step));
					}
					closeList[playerPoint.x+"_"+(playerPoint.y-step).toString()] = true;
					checking = true;
				}
				step++;
				if(!checking)
					break;
			}
			return null;
		}
	}
}