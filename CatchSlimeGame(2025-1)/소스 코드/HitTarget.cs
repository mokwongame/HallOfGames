using UnityEngine;

public class HitTarget : MonoBehaviour
{
    private GameManager gameManager;
    void Start()
    {
        gameManager = FindAnyObjectByType<GameManager>();
    }

    
    void Update()
    {
        
    }

    private void OnCollisionEnter(Collision collision)
    {
        string tag = collision.gameObject.tag;
        //Debug.Log($"{tag} is hit");

        if (tag == "Mole")
        {
            gameManager.playHit();
            Debug.Log("One point");
            gameManager.incScore();
            Debug.Log($"score = { gameManager.score }");
            Destroy(gameObject, 0.1f);
            Destroy(collision.gameObject, 0.3f);
        }
        else if (tag == "MoleKing")
        {
            gameManager.playHit();
            Debug.Log("Three points");
            gameManager.incScore2();
            Debug.Log($"score = {gameManager.score}");
            Destroy(gameObject, 0.1f);
            Destroy(collision.gameObject, 0.3f);
        }
        else if (tag == "MoleBoom")
        {
            gameManager.playHit();
            Debug.Log("Minus two point");
            gameManager.decScore2();
            Debug.Log($"score = {gameManager.score}");
            Destroy(gameObject, 0.1f);
            Destroy(collision.gameObject, 0.3f);
        }
        else if (tag == "Land")
        {
            Debug.Log("Minus one point");
            gameManager.decScore();
            Debug.Log($"score = {gameManager.score}");
            Destroy(gameObject, 0.1f);
        }
    }

}
