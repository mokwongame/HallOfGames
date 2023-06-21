using UnityEngine;

public class MakeDragon : MonoBehaviour
{
    public GameObject dragon;
    public float startTime = 1.0f;
    float makeRate = 30.0f;
    public float ymin = -2.0f;
    public float ymax = 3.0f;
    // Start is called before the first frame update
    void Start()
    {
        InvokeRepeating("makeDragonObj", startTime, makeRate);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    void makeDragonObj()
    {

        Vector3 vec3;
        vec3.x = 14.5f;
        vec3.y = Random.Range(ymin, ymax);
        vec3.z = 0.0f;
        Instantiate(dragon, vec3, transform.rotation);
    }


}