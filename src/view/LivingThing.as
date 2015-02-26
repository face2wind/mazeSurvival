package view
{
	import com.greensock.TweenLite;
	import com.greensock.easing.Linear;
	
	import face2wind.manager.item.IRender;
	import face2wind.view.BaseSprite;
	
	import model.MovingDirection;
	import model.vo.LivingThingVo;
	
	/**
	 * 生物视图对象
	 * @author face2wind
	 */
	public class LivingThing extends BaseSprite implements IRender
	{
		public function LivingThing()
		{
			super();
		}
		protected var _speed:int = 1;
		
		private var _vo:LivingThingVo;
		private var movingLocked:Boolean;
		public function set livingVo(value:LivingThingVo):void
		{
			_vo = value;
		}
		public function get livingVo():LivingThingVo
		{
			return _vo;
		}
		
		public override function set x(value:Number):void
		{
			super.x = value;
			livingVo.x = value/MapItem.WIDTH;
		}
		
		public override function set y(value:Number):void
		{
			super.y = value;
			livingVo.y = value/MapItem.HEIGHT;
		}
		
		public function rendering(step:int = 0):void
		{
			livingVo.thinking();
			moving();
		}
		
		/**
		 * 根据vo里的移动命令，开始移动 
		 */		
		private function moving():void
		{
			if(movingLocked)
				return;
			movingLocked = true;
			
			if(MovingDirection.STOP == livingVo.movingDir)
				return;
			var targetX:Number = livingVo.x;
			var targetY:Number = livingVo.y;
			switch(livingVo.movingDir){
				case MovingDirection.TOP:targetY--;break;
				case MovingDirection.BOTTOM:targetY++;break;
				case MovingDirection.LEFT:targetX--;break;
				case MovingDirection.RIGHT:targetX++;break;
				case MovingDirection.TOP_LEFT:targetX--;targetY--;break;
				case MovingDirection.TOP_RIGHT:targetX++;targetY--;break;
				case MovingDirection.BOTTOM_LEFT:targetX--;targetY++;break;
				case MovingDirection.BOTTOM_RIGHT:targetX++;targetY++;break;
			}
			var time:Number = Math.sqrt( (targetX-livingVo.x)*(targetX-livingVo.x) + (targetY-livingVo.y)*(targetY-livingVo.y)) / _speed;
			TweenLite.to(this, time, {x:targetX*MapItem.WIDTH, y:targetY*MapItem.HEIGHT,onComplete:movingStepComplete, ease:Linear.easeNone});
		}
		
		private function movingStepComplete():void
		{
			movingLocked = false;
		}
	}
}