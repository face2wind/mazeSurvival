package controller
{
	import event.SceneEvent;
	
	import face2wind.base.Controller;
	import face2wind.enum.LayerEnum;
	import face2wind.event.ParamEvent;
	import face2wind.manager.EnterFrameUtil;
	import face2wind.manager.LayerManager;
	import face2wind.manager.TimerManager;
	
	import model.SceneManager;
	import model.vo.MonsterVo;
	import model.vo.PlayerVo;
	
	import view.Scene;
	
	/**
	 * 
	 * @author face2wind
	 */
	public class MazeController extends Controller
	{
		private var survivalPlayerVo:PlayerVo = new PlayerVo();
		
		public function MazeController()
		{
			if(instance)
				throw new Error("MazeController is singleton class and allready exists!");
			instance = this;
			
			manager = SceneManager.getInstance();
			
			_scene = new Scene();
			LayerManager.getInstance().getLayer(LayerEnum.SCENE_LAYER).addChild(_scene);
			
			eventManager.bindToController(SceneEvent.PATH_SHOW_COMPLETE, onShowPathCompleteHandler);
			eventManager.bindToController(SceneEvent.RESTART_SURVIVAL, onRestartSurvivalHandler);
			
			startingSurvival();
//			manager.reStartDemo();
		}
		
		private function onRestartSurvivalHandler(e:ParamEvent):void
		{
			startingSurvival();
		}
		
		private function startingSurvival():void
		{
			manager.removePlayer(survivalPlayerVo);
			manager.cleanMap();
			
			manager.initMaze();
			survivalPlayerVo.rebuildMindMap();
			manager.addPlayer(survivalPlayerVo);
			
			var mvo:MonsterVo = new MonsterVo();
			mvo.rebuildMindMap();
			manager.addMonster(mvo);
		}
		
		private function onShowPathCompleteHandler(e:ParamEvent):void
		{
			EnterFrameUtil.delayCall(1000, reflashDemoTimerHandler);
		}
		
		private function reflashDemoTimerHandler():void
		{
			manager.reStartDemo();
		}
		
		/**
		 * 单例
		 */
		private static var instance:MazeController;
		/**
		 * 获取单例
		 */
		public static function getInstance():MazeController
		{
			if(!instance)
				instance = new MazeController();
			
			return instance;
		}
		private var _scene:Scene;
		private var manager:SceneManager;
	}
}