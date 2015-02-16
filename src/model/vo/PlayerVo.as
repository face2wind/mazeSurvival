package model.vo
{
	import face2wind.event.PropertyChangeEvent;
	
	import flash.events.EventDispatcher;
	import flash.events.IEventDispatcher;
	import flash.geom.Point;
	
	import model.MovingDirection;
	import model.SceneManager;
	
	/**
	 * 玩家信息
	 * @author face2wind
	 */
	public class PlayerVo extends LivingThingVo
	{
		public function PlayerVo()
		{
		}
		
		/**
		 * 玩家探索到的出口，null表示未探索到 
		 */		
		private var _exitPoint:Point = null;
		
		/**
		 * 可探索点 
		 */		
		private var _expolorePoints:Array = [];
		
		/**
		 * 思考，根据当前状态决定下一步该怎么做 
		 */		
		public override function thinking():void
		{
			updateMindMap();
		}
		
		/**
		 * 根据当前的视野，更新在当前玩家脑海里的地图数据 
		 */		
		private function updateMindMap():void
		{
			var updateMapData:Array = sManager.getMapDataOnPosition(x,y);
			var updateLifeData:Array = sManager.getLifeAroundPosition(x,y);
			if(_exitPoint) // 已有出口，直奔出口
				moveToPath();
			if(0 < _expolorePoints.length){
				随机去一个探索点
			}
			
			return;
			if(0 < y)
				movingDir = MovingDirection.UP;
			else
				movingDir = MovingDirection.STOP;
		}
	}
}