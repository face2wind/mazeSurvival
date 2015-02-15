package model.vo
{
	import face2wind.event.PropertyChangeEvent;
	
	import flash.events.EventDispatcher;
	import flash.events.IEventDispatcher;
	
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
			if(0 < y)
				movingDir = MovingDirection.UP;
			else
				movingDir = MovingDirection.STOP;
		}
	}
}