package model.vo
{
	import face2wind.manager.EventManager;
	
	import findPath.AStarPathFinder;
	
	import flash.geom.Point;
	import flash.utils.Dictionary;
	
	import model.MapDataType;
	import model.MovingDirection;
	import model.SceneManager;

	/**
	 * 怪物vo
	 * @author face2wind
	 */
	public class MonsterVo extends LivingThingVo
	{
		public function MonsterVo()
		{
			super();
			_pathFinder = new AStarPathFinder();
			_pathFinder.direction8 = false;
		}
		
		/**
		 * 当前任务：1 自由寻路探索，2 追杀玩家 
		 */		
		private var _curMission:int = 1;
		
		/**
		 * 玩家探索到的出口，null表示未探索到 
		 */		
		private var _exitPoint:Point = null;
		
		/**
		 * 可探索点 
		 */
		private var _expoloreDic:Dictionary = new Dictionary();
		/**
		 * 可探索点数量 
		 */		
		private var _expoloreNum:int = 0;
		
		/**
		 * 当前是否正在移动中 
		 */		
		private var curIsMoving:Boolean = false;
		
		/**
		 * 当前移动路径 
		 */		
		private var _curMovingPath:Array = null;
		
		private var _pathFinder:AStarPathFinder;
		
		/**
		 * 思考，根据当前状态决定下一步该怎么做 
		 */		
		public override function thinking():void
		{
			super.thinking();
			updateMindMap();
			movingToNext();
			movingStep();
		}
		
		/**
		 * 移动渲染 
		 */		
		private function movingStep():void
		{
			if(null == _curMovingPath || 1 > _curMovingPath.length){ //当前没有路径，不移动
				movingDir = MovingDirection.STOP;
				curIsMoving = false;
//				EventManager.getInstance().dispatchToView(new ParamEvent(SceneEvent.SHOW_EXPOLORE_LIST, _expoloreDic));
				return;
			}
			
			var nextPoint:Point = _curMovingPath[0];
			setDirection(nextPoint);
			if(nextPoint.x == x && nextPoint.y == y){ //已到达下一个点，换方向
				_curMovingPath.shift();
			}
//			EventManager.getInstance().dispatchToView(new ParamEvent(SceneEvent.SHOW_EXPOLORE_LIST, _expoloreDic));
		}
		
		/**
		 * 寻找下一个能移动的地方 
		 */		
		private function movingToNext():void
		{
//			if(_exitPoint){ // 已有出口，直奔出口
//				moveToPath(_exitPoint, true);
//				return;
//			}
			if(0 < _expoloreNum){ // 找一个最近的探索点寻路
				var minPoint:Point = null;
				var min:Number = -1;
				for each (var ePoint:Point in _expoloreDic) {
					var curPath:Array = _pathFinder.findPath(new Point(x,y), ePoint);
					var curDis:Number = curPath?curPath.length:999;  // 按寻路的路径长度判断距离远近
					//					var curDis:Number = Math.sqrt( (ePoint.x-x)*(ePoint.x-x) + (ePoint.y-y)*(ePoint.y-y) ); // 按直线距离判断远近
					if(null == minPoint || curDis < min){
						minPoint = ePoint;
						min = curDis;
					}
				}
				if(minPoint){
					moveToPath(minPoint);
				}
			}else{
				//				Message.show("无路可走了");
			}
		}
		
		/**
		 * 根据当前的视野，更新在当前玩家脑海里的地图数据 
		 */		
		private function updateMindMap():void
		{
			var updateMapData:Array = sManager.getMapDataOnPosition(x,y);
			var updateLifeData:Array = sManager.getLifeAroundPosition(x,y);
			var tmpPoint:Object;
			var exitPoint:Point = SceneManager.getInstance().exitPoint;
			for (var i:int = 0; i < updateMapData.length; i++) 
			{
				tmpPoint = updateMapData[i];
				var oldValue:int = _mindMapData[tmpPoint.x][tmpPoint.y];
				_mindMapData[tmpPoint.x][tmpPoint.y] = tmpPoint.value;
				if(tmpPoint.x == exitPoint.x && tmpPoint.y == exitPoint.y)
					_exitPoint = exitPoint;
				if(MapDataType.OBSTACLE == oldValue && // 之前未探索过
					MapDataType.CAN_EXPLORE == tmpPoint.value){ // 可探索区
					_expoloreDic[tmpPoint.x+"_"+tmpPoint.y] = new Point(tmpPoint.x, tmpPoint.y);
					++_expoloreNum;
				}
				if(MapDataType.CAN_EXPLORE != tmpPoint.value &&
					undefined != _expoloreDic[tmpPoint.x+"_"+tmpPoint.y]){ // 不是可探索区，但可探索列表里有，则删除
					delete _expoloreDic[tmpPoint.x+"_"+tmpPoint.y];
					--_expoloreNum;
				}
			}
			_pathFinder.setMapData(_mindMapData);
			
			_curMission xxx
		}
		
		/**
		 * 寻路移动到指定点 
		 * @param targetPoint 目的点
		 * @param force 是否强制终止之前未走完的路
		 */		
		private function moveToPath(targetPoint:Point, force:Boolean = false):void
		{
			if(curIsMoving && false == force) // 当前正在移动，而且没有设置强制终止，则无视
				return;
			if(_curMovingPath && 0 < _curMovingPath.length){
				var curTargetPoint:Point = _curMovingPath[_curMovingPath.length-1];
				if(targetPoint.x == curTargetPoint.x && targetPoint.y == curTargetPoint.y) // 当前已有寻路的终点一样，忽略
					return;
			}
			_curMovingPath = _pathFinder.findPath(new Point(x,y), targetPoint);
			if(null == _curMovingPath || 2 > _curMovingPath.length) // 至少有两个点，才算寻路成功
				return;
			
			curIsMoving = true;
			
//			EventManager.getInstance().dispatchToView(new ParamEvent(SceneEvent.SHOW_EXPOLORE_LIST, _expoloreDic));
//			trace("go to point("+targetPoint.x+", "+targetPoint.y+")");
		}
	}
}