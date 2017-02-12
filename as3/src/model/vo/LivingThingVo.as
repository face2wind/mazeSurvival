package model.vo
{
	import face2wind.customUIComponents.Message;
	import face2wind.event.PropertyChangeEvent;
	
	import flash.events.EventDispatcher;
	import flash.events.IEventDispatcher;
	import flash.geom.Point;
	
	import model.MapDataType;
	import model.MovingDirection;
	import model.SceneManager;
	
	/**
	 * 场景内生物的信息
	 * @author face2wind
	 */
	public class LivingThingVo extends EventDispatcher
	{
		public function LivingThingVo()
		{
			_id = curMaxID;
			++ curMaxID;
		}
		
		/**
		 * 重新构建初始地图，宽高获取场景宽高，地图所有地方重置为不可走 
		 */		
		public function rebuildMindMap():void
		{
			var size:Point = SceneManager.getInstance().getSceneSize();
			_mindMapData = [];
			for (var i:int = 0; i < size.x; i++) {
				_mindMapData.push([]);
				for (var j:int = 0; j < size.y; j++) 
					_mindMapData[i].push(MapDataType.OBSTACLE);
			}
		}
		
		/**
		 * 这只生物脑海里的地图全貌，默认是全部不可走，通过探索慢慢接近真实地图 
		 */		
		protected var _mindMapData:Array;
		
		/**
		 * X坐标（格子）
		 */
		private var _x:int;
		/**
		 * X坐标（格子）
		 */
		public function get x():int
		{
			return _x;
		}
		/**
		 * X坐标（格子）
		 */
		public function set x(value:int):void
		{
			if(_x == value)
				return;
			
			var _oldValue:* = _x;
			_x = value;
			dispatchEvent(PropertyChangeEvent.createUpdateEvent("x", _oldValue, _x));
		}
		
		/**
		 * Y坐标（格子）
		 */
		private var _y:int;
		/**
		 * Y坐标（格子）
		 */
		public function get y():int
		{
			return _y;
		}
		/**
		 * Y坐标（格子）
		 */
		public function set y(value:int):void
		{
			if(_y == value)
				return;
			
			var _oldValue:* = _y;
			_y = value;
			dispatchEvent(PropertyChangeEvent.createUpdateEvent("y", _oldValue, _y));
		}
		
		/**
		 * AI等级
		 */
		private var _aiLevel:int;
		/**
		 * AI等级
		 */
		public function get aiLevel():int
		{
			return _aiLevel;
		}
		/**
		 * AI等级
		 */
		public function set aiLevel(value:int):void
		{
			if(_aiLevel == value)
				return;
			
			var _oldValue:* = _aiLevel;
			_aiLevel = value;
			dispatchEvent(PropertyChangeEvent.createUpdateEvent("aiLevel", _oldValue, _aiLevel));
		}
		
		/**
		 * 当前移动方向MovingDirection
		 */
		private var _movingDir:int = MovingDirection.STOP;
		/**
		 * 当前移动方向MovingDirection
		 */
		public function get movingDir():int
		{
			return _movingDir;
		}
		/**
		 * 当前移动方向MovingDirection
		 */
		public function set movingDir(value:int):void
		{
			if(_movingDir == value)
				return;
			
			var _oldValue:* = _movingDir;
			_movingDir = value;
			dispatchEvent(PropertyChangeEvent.createUpdateEvent("movingDir", _oldValue, _movingDir));
		}
		
		private static var curMaxID:int = 1;
		/**
		 * 生物唯一ID
		 */
		private var _id:int = 0;
		/**
		 * 生物唯一ID
		 */
		public function get id():int
		{
			return _id;
		}
		
		/**
		 * 把当前运动方向设置为当前点跟指定点构成的方向
		 * @param nextPoint
		 */		
		protected function setDirection(nextPoint:Point):void
		{
			if(x < nextPoint.x && y < nextPoint.y)
				movingDir = MovingDirection.BOTTOM_RIGHT;
			if(x < nextPoint.x && y == nextPoint.y)
				movingDir = MovingDirection.RIGHT;
			if(x < nextPoint.x && y > nextPoint.y)
				movingDir = MovingDirection.TOP_RIGHT;
			
			if(x > nextPoint.x && y < nextPoint.y)
				movingDir = MovingDirection.BOTTOM_LEFT;
			if(x > nextPoint.x && y == nextPoint.y)
				movingDir = MovingDirection.LEFT;
			if(x > nextPoint.x && y > nextPoint.y)
				movingDir = MovingDirection.TOP_LEFT;
			
			if(x == nextPoint.x && y < nextPoint.y)
				movingDir = MovingDirection.BOTTOM;
			if(x == nextPoint.x && y == nextPoint.y)
				movingDir = MovingDirection.STOP;
			if(x == nextPoint.x && y > nextPoint.y)
				movingDir = MovingDirection.TOP;
		}
		
		protected var sManager:SceneManager = SceneManager.getInstance();
		
		/**
		 * 当前渲染步数，外部渲染器设置 
		 */		
		public var step:int = 0;
		
		/**
		 * 思考，根据当前状态决定下一步该怎么做 
		 */		
		public function thinking():void
		{
			//交给子类覆盖
		}
	}
}