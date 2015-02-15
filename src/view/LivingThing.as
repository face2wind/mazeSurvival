package view
{
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
		private var _speed:int = 1;
		
		private var _vo:LivingThingVo;
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
			var targetDir:int = livingVo.movingDir;
			if(-1 != livingVo.preDir){ // 之前的位置未稳定下来
				if(Math.abs(x-livingVo.x*MapItem.WIDTH) < 4){
					x = livingVo.x*MapItem.WIDTH;
					livingVo.preDir = -1;
				}else{
					targetDir = livingVo.preDir;
				}
				if(Math.abs(y-livingVo.y*MapItem.HEIGHT) < 4){
					y = livingVo.y*MapItem.HEIGHT;
					livingVo.preDir = -1;
				}else{
					targetDir = livingVo.preDir;
				}
			}
			if(MovingDirection.STOP == targetDir)
				return;
			switch(targetDir)
			{
				case MovingDirection.UP:y=y-_speed;break;
				case MovingDirection.DOWN:y=y+_speed;break;
				case MovingDirection.LEFT:x=x-_speed;break;
				case MovingDirection.RIGHT:x=x+_speed;break;
			}
		}
	}
}