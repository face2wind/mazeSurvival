package model.vo
{
	import face2wind.event.PropertyChangeEvent;
	
	import flash.events.EventDispatcher;
	import flash.events.IEventDispatcher;
	
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
			if(MovingDirection.STOP == _oldValue || MovingDirection.STOP == _movingDir)
				preDir = _oldValue;
			dispatchEvent(PropertyChangeEvent.createUpdateEvent("movingDir", _oldValue, _movingDir));
		}
		
		/**
		 * 前一个方向 
		 */		
		public var preDir:int = -1;
		
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
		
		protected var sManager:SceneManager = SceneManager.getInstance();
		
		/**
		 * 思考，根据当前状态决定下一步该怎么做 
		 */		
		public function thinking():void
		{
			//交给子类覆盖
		}
	}
}