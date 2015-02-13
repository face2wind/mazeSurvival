package model
{
	import event.SceneEvent;
	
	import face2wind.event.ParamEvent;
	
	import flash.events.EventDispatcher;
	import flash.events.IEventDispatcher;
	
	import view.MapItem;
	
	/**
	 * 数据管理
	 * @author face2wind
	 */
	public class SceneManager extends EventDispatcher
	{
		public function SceneManager()
		{
			if(instance)
				throw new Error("SceneManager is singleton class and allready exists!");
			instance = this;
		}
		
		/**
		 * 单例
		 */
		private static var instance:SceneManager;
		/**
		 * 获取单例
		 */
		public static function getInstance():SceneManager
		{
			if(!instance)
				instance = new SceneManager();
			
			return instance;
		}
		
		/**
		 * 地图数据 
		 */		
		private var mapData:Array;
		/**
		 * 场景宽度（格子数） 
		 */		
		private var _sceneWidth:int = 7;
		/**
		 * 场景高度（格子数） 
		 */		
		private var _sceneHeight:int = 5;
		
		/**
		 * 获取地图数据 
		 * @return 
		 */		
		public function getMapData():Array
		{
			return mapData;
		}
		
		/**
		 * 重新开始整个迷宫 
		 */		
		public function reStartDemo():void
		{
			mapData = [];
			for (var i:int = 0; i < _sceneWidth; i++) {
				mapData[i] = [];
				for (var j:int = 0; j < _sceneHeight; j++) 
					mapData[i][j] = MapDataType.GROUND;//MapDataType.getRandomType();
			}
			mapData[3][1] = MapDataType.OBSTACLE;
			mapData[3][2] = MapDataType.OBSTACLE;
			mapData[3][3] = MapDataType.OBSTACLE;
			dispatchEvent(new ParamEvent(SceneEvent.RESTART_DEMO));
		}
	}
}