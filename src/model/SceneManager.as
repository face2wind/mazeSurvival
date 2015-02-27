package model
{
	import event.SceneEvent;
	
	import face2wind.event.ParamEvent;
	import face2wind.manager.EventManager;
	
	import flash.events.EventDispatcher;
	import flash.events.IEventDispatcher;
	import flash.geom.Point;
	import flash.utils.Dictionary;
	
	import model.vo.MonsterVo;
	import model.vo.PlayerVo;
	
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
			
			playerVoDic = new Dictionary();
			monsterVoDic = new Dictionary();
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
		 * 获取场景宽高（格子数） 
		 * @return 
		 */		
		public function getSceneSize():Point
		{
			return new Point(_sceneWidth,_sceneHeight);
		}
		
		/**
		 * 地图数据 
		 */		
		private var mapData:Array;
		/**
		 * 场景宽度（格子数） 
		 */		
		private var _sceneWidth:int = 25;//72;
		/**
		 * 场景高度（格子数） 
		 */		
		private var _sceneHeight:int = 18;//42;
		/**
		 * 玩家字典（逃生者） 
		 */		 
		private var playerVoDic:Dictionary;
		/**
		 * 怪物字典 
		 */		
		private var monsterVoDic:Dictionary;
		
		private var _enterPoint:Point;
		/**
		 * 入口 
		 * @return 
		 */		
		public function get enterPoint():Point
		{
			return _enterPoint;
		}
		
		private var _exitPoint:Point;
		/**
		 * 出口 
		 * @return 
		 */		
		public function get exitPoint():Point
		{
			return _exitPoint;
		}
		
		/**
		 * 获取地图数据 
		 * @return 
		 */		
		public function getMapData():Array
		{
			return mapData;
		}
		
		public function addPlayer(playerVo:PlayerVo):void
		{
			playerVoDic[playerVo.id] = playerVo;
			dispatchEvent(new ParamEvent(SceneEvent.CREATE_PLAYER, playerVo));
		}
		
		public function removePlayer(playerVo:PlayerVo):void
		{
			delete playerVoDic[playerVo.id];
			dispatchEvent(new ParamEvent(SceneEvent.REMOVE_PLAYER, playerVo));
		}
		
		public function addMonster(monsterVo:MonsterVo):void
		{
			monsterVoDic[monsterVo.id] = monsterVo;
			dispatchEvent(new ParamEvent(SceneEvent.CREATE_MONSTER, monsterVo));
		}
		
		private function createRandomMap():void
		{
			mapData = [];
			for (var i:int = 0; i < _sceneWidth; i++) {
				mapData[i] = [];
				for (var j:int = 0; j < _sceneHeight; j++){ 
					if(0 == i || _sceneWidth-1 == i) // 两边不要生成障碍
						mapData[i][j] = MapDataType.GROUND;
					else
						mapData[i][j] = MapDataType.getRandomType();
				}
			}
			var maxX:int = _sceneWidth-1;
			var mapH:int = _sceneHeight;
			var randomY1:int = (Math.random()+mapH/2)%mapH;
			var randomY2:int = Math.random()*mapH;
			_enterPoint = new Point(0,randomY1);
			_exitPoint = new Point(maxX, randomY2);
		}
		
		public function initMaze():void
		{
			createRandomMap();
			dispatchEvent(new ParamEvent(SceneEvent.REBUILD_MAP));
		}
		
		/**
		 * 重新开始整个迷宫 
		 */		
		public function reStartDemo():void
		{
			initMaze();
			dispatchEvent(new ParamEvent(SceneEvent.RESTART_DEMO));
		}
		
		/**
		 * 获取指定位置的视野能观察到的地图数据 
		 * @param x
		 * @param y
		 */		
		public function getMapDataOnPosition(x:int, y:int):Array
		{
			var viewMapData:Array = [];
			if(0 > x || 0 > y || _sceneHeight <= y || _sceneWidth <= x)
				return viewMapData;
			viewMapData.push({x:x,y:y,value:MapDataType.GROUND});
			// 四个方向
			var xpos:int = x;
			var ypos:int = y-1;
			while(0 < ypos){ // 上
				if(MapDataType.OBSTACLE == mapData[xpos][ypos]){
					viewMapData.push({x:xpos,y:ypos,value:MapDataType.OBSTACLE});
					break;
				}else{
					viewMapData.push({x:xpos,y:ypos,value:MapDataType.GROUND});
				}
				if(0 < x){
					if(MapDataType.OBSTACLE == mapData[xpos-1][ypos])
						viewMapData.push({x:xpos-1,y:ypos,value:MapDataType.OBSTACLE});
					else
						viewMapData.push({x:xpos-1,y:ypos,value:MapDataType.CAN_EXPLORE}); // 旁边的格子若可走，则设置为可探索点
				}
				if(_sceneWidth-1 > x){
					if(MapDataType.OBSTACLE == mapData[xpos+1][ypos])
						viewMapData.push({x:xpos+1,y:ypos,value:MapDataType.OBSTACLE});
					else
						viewMapData.push({x:xpos+1,y:ypos,value:MapDataType.CAN_EXPLORE}); // 旁边的格子若可走，则设置为可探索点
				}
				ypos--;
			}
			ypos = y+1;
			while(_sceneHeight > ypos){ // 下
				if(MapDataType.OBSTACLE == mapData[xpos][ypos]){
					viewMapData.push({x:xpos,y:ypos,value:MapDataType.OBSTACLE});
					break;
				}else{
					viewMapData.push({x:xpos,y:ypos,value:MapDataType.GROUND});
				}
				if(0 < x){
					if(MapDataType.OBSTACLE == mapData[xpos-1][ypos])
						viewMapData.push({x:xpos-1,y:ypos,value:MapDataType.OBSTACLE});
					else
						viewMapData.push({x:xpos-1,y:ypos,value:MapDataType.CAN_EXPLORE}); // 旁边的格子若可走，则设置为可探索点
				}
				if(_sceneWidth-1 > x){
					if(MapDataType.OBSTACLE == mapData[xpos+1][ypos])
						viewMapData.push({x:xpos+1,y:ypos,value:MapDataType.OBSTACLE});
					else
						viewMapData.push({x:xpos+1,y:ypos,value:MapDataType.CAN_EXPLORE}); // 旁边的格子若可走，则设置为可探索点
				}
				ypos++;
			}
			xpos = x-1;
			ypos = y;
			while(0 < xpos){ // 左
				if(MapDataType.OBSTACLE == mapData[xpos][ypos]){
					viewMapData.push({x:xpos,y:ypos,value:MapDataType.OBSTACLE});
					break;
				}else{
					viewMapData.push({x:xpos,y:ypos,value:MapDataType.GROUND});
				}
				if(0 < y){
					if(MapDataType.OBSTACLE == mapData[xpos][ypos-1])
						viewMapData.push({x:xpos,y:ypos-1,value:MapDataType.OBSTACLE});
					else
						viewMapData.push({x:xpos,y:ypos-1,value:MapDataType.CAN_EXPLORE}); // 旁边的格子若可走，则设置为可探索点
				}
				if(_sceneHeight-1 > y){
					if(MapDataType.OBSTACLE == mapData[xpos][ypos+1])
						viewMapData.push({x:xpos,y:ypos+1,value:MapDataType.OBSTACLE});
					else
						viewMapData.push({x:xpos,y:ypos+1,value:MapDataType.CAN_EXPLORE}); // 旁边的格子若可走，则设置为可探索点
				}
				xpos--;
			}
			xpos = x+1;
			while(_sceneWidth >xpos){ // 右
				if(MapDataType.OBSTACLE == mapData[xpos][ypos]){
					viewMapData.push({x:xpos,y:ypos,value:MapDataType.OBSTACLE});
					break;
				}else{
					viewMapData.push({x:xpos,y:ypos,value:MapDataType.GROUND});
				}
				if(0 < y){
					if(MapDataType.OBSTACLE == mapData[xpos][ypos-1])
						viewMapData.push({x:xpos,y:ypos-1,value:MapDataType.OBSTACLE});
					else
						viewMapData.push({x:xpos,y:ypos-1,value:MapDataType.CAN_EXPLORE}); // 旁边的格子若可走，则设置为可探索点
				}
				if(_sceneHeight-1 > y){
					if(MapDataType.OBSTACLE == mapData[xpos][ypos+1])
						viewMapData.push({x:xpos,y:ypos+1,value:MapDataType.OBSTACLE});
					else
						viewMapData.push({x:xpos,y:ypos+1,value:MapDataType.CAN_EXPLORE}); // 旁边的格子若可走，则设置为可探索点
				}
				xpos++;
			}
			return viewMapData;
		}
		
		/**
		 * 获取指定位置视野能观察到的生物（包括人和怪物） 
		 * @param x
		 * @param y
		 * @return 
		 */		
		public function getLifeAroundPosition(x:int, y:int):Array
		{
			// TODO Auto Generated method stub
			return null;
		}
		
		/**
		 * 清空当前地图 
		 */		
		public function cleanMap():void
		{
			// TODO Auto Generated method stub
			
		}
	}
}