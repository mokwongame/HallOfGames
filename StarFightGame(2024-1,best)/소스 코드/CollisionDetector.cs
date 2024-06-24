using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class CollisionDetector : MonoBehaviour
{
    void OnTriggerEnter2D(Collider2D other)
    {
        if (other.CompareTag("Rock"))
        {
            // Rock과 충돌하면 EndGameScene으로 전환
            SceneManager.LoadScene("EndGameScene");
        }
    }
}
