package findPath
{
	import face2wind.lib.Debuger;
	import face2wind.lib.ObjectPool;
	
	import flash.geom.Point;
	import flash.utils.Dictionary;
	
	import model.MapDataType;
	
	/**
	 * A*寻路
	 * @author face2wind
	 */
	public class AStarPathFinder implements IPathFinder
	{
		public function AStarPathFinder()
		{
		}
		public static const CAN_MOVING_AREA:int = 0;
		public static const CAN_NOT_MOVING_AREA:int = 1;
		/**
		 * 地图数据 
		 */		
		private var _mapData:Array;
		/**
		 * 开启列表 
		 */		
		private var openList:Array;
		/**
		 * 关闭列表 
		 */		
		private var closeList:Array;
		/**
		 * 当前寻路的起始点 
		 */		
		private var startAPoint:AStarPoint;
		/**
		 * 当前寻路的终点 
		 */		
		private var endAPoint:AStarPoint;
		
		private var mapWidth:int;
		
		private var mapHeight:int;
		
		public function setMapData(data:Array):void
		{
			if(null == data || 0 == data.length)
				return;
			mapWidth = data.length;
			mapHeight = data[0].length;
			_mapData = [];
			for (var i:int = 0; i < mapWidth; i++) 
			{
				_mapData[i] = [];
				for (var j:int = 0; j < mapHeight; j++) 
				{
					var p:AStarPoint = ObjectPool.getObject(AStarPoint);
					p.x = i;
					p.y = j;
					p.f = p.g = p.h = 0;
					p.mapType = data[i][j];
					p.prePoint = null;
					_mapData[i].push(p);
				}
			}
		}
		
		/**
		 * 放入开放列表，插入排序，保证列表最后一个元素的F值最小 
		 * @param newPoint
		 */		
		private function addToOpenList(newPoint:AStarPoint):void
		{
			if(-1 != openList.indexOf(newPoint))
				return;
			if(-1 != closeList.indexOf(newPoint))
				return;
			for (var i:int = 0; i < openList.length; i++){
				var curPoint:AStarPoint = openList[i];
				if(curPoint.f <= newPoint.f && curPoint.level >= newPoint.level)
					break;
			}
			openList.splice(i, 0 ,newPoint);
		}
		
		/**
		 * 返回指定点的周围节点
		 * @param curPoint
		 * @return 
		 */		
		private function getSurroundingPoint(curPoint:AStarPoint):Array
		{
			var surroundArr:Array = [];
			var width:int = _mapData.length-1;
			var height:int = _mapData[0].length-1;
			
			if(0 < curPoint.x  && CAN_MOVING_AREA == _mapData[curPoint.x-1][curPoint.y].mapType) // 左
					surroundArr.push(_mapData[curPoint.x-1][curPoint.y]);
			if(width > curPoint.x && CAN_MOVING_AREA == _mapData[curPoint.x+1][curPoint.y].mapType ) // 右
				surroundArr.push(_mapData[curPoint.x+1][curPoint.y]);
			if(0 < curPoint.y && CAN_MOVING_AREA == _mapData[curPoint.x][curPoint.y-1].mapType ) // 上
				surroundArr.push(_mapData[curPoint.x][curPoint.y-1]);
			if(height > curPoint.y && CAN_MOVING_AREA == _mapData[curPoint.x][curPoint.y+1].mapType ) // 下
				surroundArr.push(_mapData[curPoint.x][curPoint.y+1]);
			
			if( 0 < curPoint.x && 0 < curPoint.y &&
				CAN_MOVING_AREA == _mapData[curPoint.x-1][curPoint.y-1].mapType && // 左上
				!(CAN_NOT_MOVING_AREA == _mapData[curPoint.x-1][curPoint.y].mapType&&
				CAN_NOT_MOVING_AREA == _mapData[curPoint.x][curPoint.y-1].mapType) ) // 没有被两边夹住
				surroundArr.push(_mapData[curPoint.x-1][curPoint.y-1]);
			if(width > curPoint.x && 0 < curPoint.y &&
				CAN_MOVING_AREA == _mapData[curPoint.x+1][curPoint.y-1].mapType && // 右上
				!(CAN_NOT_MOVING_AREA == _mapData[curPoint.x][curPoint.y-1].mapType &&
					CAN_NOT_MOVING_AREA == _mapData[curPoint.x+1][curPoint.y].mapType) ) // 没有被两边夹住
				surroundArr.push(_mapData[curPoint.x+1][curPoint.y-1]);
			if(width > curPoint.x && height > curPoint.y &&
				CAN_MOVING_AREA == _mapData[curPoint.x+1][curPoint.y+1].mapType && // 右下
				!(CAN_NOT_MOVING_AREA == _mapData[curPoint.x][curPoint.y+1].mapType &&
				CAN_NOT_MOVING_AREA == _mapData[curPoint.x+1][curPoint.y].mapType) ) // 没有被两边夹住
				surroundArr.push(_mapData[curPoint.x+1][curPoint.y+1]);
			if(0 < curPoint.x && height > curPoint.y &&
				CAN_MOVING_AREA == _mapData[curPoint.x-1][curPoint.y+1].mapType && // 左下
				!(CAN_NOT_MOVING_AREA == _mapData[curPoint.x][curPoint.y+1].mapType &&
				CAN_NOT_MOVING_AREA == _mapData[curPoint.x-1][curPoint.y].mapType) ) // 没有被两边夹住
				surroundArr.push(_mapData[curPoint.x-1][curPoint.y+1]);
			return surroundArr;
		}
		
		/**
		 * 计算当前节点的寻路数值 G, H, F
		 * @param preAp 前一个节点（父节点）
		 * @param curAp 要计算的节点
		 */			
		private function calculateGHF(preAp:AStarPoint, curAp:AStarPoint):void
		{
			if(-1 != closeList.indexOf(curAp)) // 当前节点已在关闭列表，忽略
				return;
			var targetG:int = 0;
			var targetH:int = 0;
			var targetLevel:int = 0;
			if(preAp){
				targetLevel = preAp.level + 1;
				if(preAp.x != curAp.x && preAp.y != curAp.y) // 对角线上的相邻点，距离用14
					targetG = preAp.g + 14;
				else
					targetG = preAp.g + 10;
			}
			targetH = (Math.abs(endAPoint.x-curAp.x)+Math.abs(endAPoint.y-curAp.y))*10; // 曼哈顿方法求H值
			
			// 开放列表里已有这个节点，而且旧的F值小于新的F值，说明旧的路线更优，忽略
			if(-1 != openList.indexOf(curAp) && (targetG+targetH) > curAp.f )
				return;
			
//			if(preAp)
//				trace("("+curAp.x+","+curAp.y+") -> ("+preAp.x+","+preAp.y+")");
			curAp.level = targetLevel;
			curAp.prePoint = preAp;
			curAp.h = targetH;
			curAp.g = targetG;
			curAp.f = targetH+targetG;
		}
		
		/**
		 * 根据寻路结果生成对应的路径 
		 * @return 
		 */		
		private function createFinalPath():Array
		{
			var curAp:AStarPoint = endAPoint;
			if(null == curAp)
				return null;
			var path:Array = [];
			while(curAp.prePoint){
				if(-1 != path.indexOf(curAp)) // 存在交叉路径，说明寻的路有问题
					return null;
				path.unshift(curAp);
				curAp = curAp.prePoint;
			}
			if(curAp == startAPoint)
				path.push(curAp);
			else
				return null;
			
			var realPath:Array = [];
			var msg:String;
			for (var i:int = 0; i < path.length; i++) {
				realPath.push(new Point(path[i].x, path[i].y));
				msg += ("("+path[i].x+","+path[i].y+") -> ");
			}
			return realPath;
		}
		
		/**
		 * 检测指定点是否在地图里 
		 * @param p
		 * @return 
		 */		
		private function pointInMap(p:Point):Boolean
		{
			if(0 <= p.x && mapWidth > p.x && 0 <= p.y && mapHeight > p.y)
				return true;
			else
				return false;
		}
		
		public function findPath(startPoint:Point, endPoint:Point):Array
		{
			if(null == _mapData || 0 == _mapData.length){
				Debuger.show(Debuger.BASE_LIB, "AStarPathFinder.findPath : mapData was null or illegal");
				return null;
			}
			if(!pointInMap(startPoint) || !pointInMap(endPoint) ){
				Debuger.show(Debuger.BASE_LIB, "AStarPathFinder.findPath : startPoint or endPoint value was illegal");
				return null;
			}
			var hasFound:Boolean = false;
			openList = [];
			closeList = [];
			startAPoint = _mapData[startPoint.x][startPoint.y];
			endAPoint =  _mapData[endPoint.x][endPoint.y];
			calculateGHF(null, startAPoint);
			addToOpenList(startAPoint);
			while(openList.length){
				var curPoint:AStarPoint = openList.pop();
//				trace("current point("+curPoint.x+","+curPoint.y+") F = " + curPoint.f);
				if(-1 == closeList.indexOf(curPoint))
					closeList.push(curPoint);
				var surroundPoints:Array = getSurroundingPoint(curPoint);
				for (var i:int = 0; i < surroundPoints.length; i++){
					var surroundPoint:AStarPoint = surroundPoints[i];
					calculateGHF(curPoint, surroundPoint);
					addToOpenList(surroundPoint);
					if(surroundPoint == endAPoint){
						hasFound = true;
						break;
					}
				}
				if(hasFound)
					break;
			}
			return createFinalPath();
		}
	}
}
class AStarPoint
{
	public function AStarPoint(){}
	public var x:int;
	public var y:int;
	public var f:int;
	public var g:int;
	public var h:int;
	/**
	 * 层数，数字越大代表是越新的路径，开放列表排序把新生成的列表优先检测 （深度优先）
	 */	
	public var level:int = 0;
	public var mapType:int;
	public var prePoint:AStarPoint;
}