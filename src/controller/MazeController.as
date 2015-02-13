package controller
{
	import face2wind.base.Controller;
	import face2wind.enum.LayerEnum;
	import face2wind.manager.LayerManager;
	
	import model.SceneManager;
	
	import view.Scene;
	
	/**
	 * 
	 * @author face2wind
	 */
	public class MazeController extends Controller
	{
		public function MazeController()
		{
			if(instance)
				throw new Error("MazeController is singleton class and allready exists!");
			instance = this;
			
			manager = SceneManager.getInstance();
			
			_scene = new Scene();
			LayerManager.getInstance().getLayer(LayerEnum.SCENE_LAYER).addChild(_scene);
			
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