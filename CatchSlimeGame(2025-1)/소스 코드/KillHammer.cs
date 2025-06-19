using UnityEngine;

public class KillHammer : MonoBehaviour
{
    public float killTime = 2.0f;
    void Start()
    {
        Destroy(gameObject, killTime);
    }


    void Update()
    {

    }
}
