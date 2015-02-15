package model.vo
{
	/**
	 * 怪物vo
	 * @author face2wind
	 */
	public class MonsterVo extends LivingThingVo
	{
		public function MonsterVo()
		{
			super();
		}
		
		/**
		 * 思考，根据当前状态决定下一步该怎么做 
		 */		
		public override function thinking():void
		{
			
		}
	}
}